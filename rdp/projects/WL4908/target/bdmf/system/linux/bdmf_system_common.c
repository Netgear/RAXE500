/*
 * <:copyright-BRCM:2019:DUAL/GPL:standard
 * 
 *    Copyright (c) 2019 Broadcom 
 *    All Rights Reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation (the "GPL").
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * 
 * A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 * writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * :>
 */


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/ctype.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/in.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/delay.h>
#include <linux/fdtable.h>
#include <linux/vmalloc.h>
#include <linux/device.h>

#include "bdmf_system.h"
#include "bdmf_system_common.h"
#include "bcm_pkt_lengths.h"

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

extern struct device *rdp_dummy_dev;

static uint32_t sysb_headroom[bdmf_sysb_type__num_of];

/** Set headroom size for system buffer
 * \param[in]   sysb_type   System buffer type
 * \param[in]   headroom    Headroom size
 */
void bdmf_sysb_headroom_size_set(bdmf_sysb_type sysb_type, uint32_t headroom)
{
    sysb_headroom[sysb_type] = BCM_PKT_HEADROOM;
}

/*
 * Memory allocation
 */

void *bdmf_alloc(size_t size)
{
    /*bdmf_alloc() may be called while the bdmf lock (a spinlock) is held.
     *This means we can't use GFP_KERNEL allocation, since that might sleep
     *(sleeping is not allowed while in an atomic context). We should use
     *GFP_ATOMIC.*/
    return kmalloc(size, GFP_ATOMIC);
}

void bdmf_free(void *p)
{
    if (!p)
	return;

    kfree(p);
}

/* MIPS */
#if defined(CONFIG_MIPS)

void *bdmf_alloc_uncached(int size, bdmf_phys_addr_t *phys_addr_p)
{
    void *mem_p = kmalloc(size, GFP_ATOMIC);

    if (mem_p != NULL)
    {
        bdmf_dcache_flush((uint32_t)mem_p, size);

        mem_p = (void *)KSEG1ADDR(mem_p);
    }

    *phys_addr_p = CPHYSADDR(mem_p);

    return mem_p;
}

void bdmf_free_uncached(void *virt_p, bdmf_phys_addr_t phys_addr, int size)
{
    kfree((void *)KSEG0ADDR(virt_p));
}

/* ARM */
#elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)

void *bdmf_alloc_uncached(int size, bdmf_phys_addr_t *phys_addr_p)
{
    dma_addr_t dma_addr;
    void *ret;

    if (rdp_dummy_dev == NULL)
        return NULL;

    ret = dma_alloc_coherent(rdp_dummy_dev, size, &dma_addr, GFP_ATOMIC);
    *phys_addr_p = (bdmf_phys_addr_t)(dma_addr);
    return ret;
}

void bdmf_free_uncached(void *virt_p, bdmf_phys_addr_t phys_addr, int size)
{
    dma_addr_t dma_addr = (dma_addr_t)phys_addr;

    if (rdp_dummy_dev == NULL)
        return;

    dma_free_coherent(rdp_dummy_dev, size, virt_p, dma_addr);
}

#else

#error "Unsupported platform for bdmf_alloc_uncached/bdmf_free_uncached"

#endif

/*
 * Tasks
 */

int bdmf_task_create(const char *name, int priority, int stack,
    int (*handler)(void *arg), void *arg, bdmf_task *ptask)
{
    struct task_struct *t;
    t = kthread_run(handler, arg, name);
    if (t == ERR_PTR(-ENOMEM))
        return BDMF_ERR_NOMEM;
    *ptask = t;
    return 0;
}

int bdmf_task_destroy(bdmf_task task)
{
    kthread_stop(task);
    return 0;
}

const char *bdmf_task_get_name(bdmf_task task, char *name)
{
    sprintf(name, "%s (%d)", task->comm, task->pid);
    return name;
}

const bdmf_task bdmf_task_get_current(void)
{
    return current;
}

/*
 * Mutex support
 */

void bdmf_mutex_init(bdmf_mutex *pmutex)
{
    mutex_init(pmutex);
}

int bdmf_mutex_lock(bdmf_mutex *pmutex)
{
    WARN(in_interrupt(), "Attempt to take mutex in interrupt context\n");
    if (mutex_lock_interruptible(pmutex) == -EINTR)
        return BDMF_ERR_INTR;
    return 0;
}

void bdmf_mutex_unlock(bdmf_mutex *pmutex)
{
    mutex_unlock(pmutex);
}

/*
 * Recursive mutex support
 */

static DEFINE_SPINLOCK(ta_mutex_lock);

void bdmf_ta_mutex_init(bdmf_ta_mutex *pmutex)
{
    pmutex->pid = -1;
    pmutex->count = 0;
    mutex_init(&pmutex->m);
}

int bdmf_ta_mutex_lock(bdmf_ta_mutex *pmutex)
{
    unsigned long flags;
    BUG_ON(!current);

    WARN(in_interrupt(), "Attempt to take ta_mutex in interrupt context\n");

    spin_lock_irqsave(&ta_mutex_lock, flags);
    if (pmutex->pid == current->pid)
    {
        ++pmutex->count;
        spin_unlock_irqrestore(&ta_mutex_lock, flags);
        return 0;
    }
    spin_unlock_irqrestore(&ta_mutex_lock, flags);

    /* not-recurring request */
    if (mutex_lock_interruptible(&pmutex->m) == -EINTR)
        return BDMF_ERR_INTR;

    pmutex->pid = current->pid;
    pmutex->count = 1;

    return 0;
}

void bdmf_ta_mutex_unlock(bdmf_ta_mutex *pmutex)
{
    BUG_ON(!current);
    BUG_ON(pmutex->pid != current->pid);
    BUG_ON(pmutex->count < 1);
    if (--pmutex->count == 0)
    {
        pmutex->pid = -1;
        mutex_unlock(&pmutex->m);
    }
}

void bdmf_ta_mutex_delete(bdmf_ta_mutex *pmutex)
{
}

/*
 * Recursive fastlock support
 */
#ifndef NR_CPUS
#define NR_CPUS 2
#endif
typedef struct {
    int refcnt[NR_CPUS];
    spinlock_t lock;
} bdmf_linux_reent_fastlock;

void bdmf_reent_fastlock_init(bdmf_reent_fastlock *lock)
{
    bdmf_linux_reent_fastlock *reent_fastlock = (bdmf_linux_reent_fastlock *)lock;

    memset(lock, 0, sizeof(bdmf_reent_fastlock));
    BUG_ON(sizeof(bdmf_linux_reent_fastlock) > sizeof(bdmf_reent_fastlock));
    spin_lock_init(&reent_fastlock->lock);
}

int bdmf_reent_fastlock_lock(bdmf_reent_fastlock *lock)
{
    bdmf_linux_reent_fastlock *reent_fastlock = (bdmf_linux_reent_fastlock *)lock;
    int cpu_id;

    cpu_id = get_cpu();/* Disable preemption */
    BUG_ON(cpu_id < 0 || cpu_id >= NR_CPUS);
    BUG_ON(in_irq());
    reent_fastlock->refcnt[cpu_id]++;
    if (reent_fastlock->refcnt[cpu_id] == 1) /* First time lock is taken */
        spin_lock_bh(&reent_fastlock->lock);
    return 0;
}

void bdmf_reent_fastlock_unlock(bdmf_reent_fastlock *lock)
{
    bdmf_linux_reent_fastlock *reent_fastlock = (bdmf_linux_reent_fastlock *)lock;
    int cpu_id;

    cpu_id = smp_processor_id();
    BUG_ON(cpu_id < 0 || cpu_id >= NR_CPUS);
    BUG_ON(!reent_fastlock->refcnt[cpu_id]); /* Bad call - unlock w/o lock */
    reent_fastlock->refcnt[cpu_id]--;
    if (!reent_fastlock->refcnt[cpu_id]) /* Last time - release the lock */
        spin_unlock_bh(&reent_fastlock->lock);
    put_cpu(); /* Enable preemption */
}

/*
 * Print to the current process' stdout
 */

static int _bdmf_write(struct file *fd, const unsigned char *buf, int len)
{
    int len0 = len;
    while(len)
    {
        int lw;

        lw = kernel_write(fd, (char *)buf, len, &fd->f_pos);
        if (lw < 0)
            break;
        len -= lw;
        if (len)
        {
            if (msleep_interruptible(1) < 0)
                break;
            buf += lw;
        }
    }
    return (len0 - len);
}

int bdmf_vprint(const char *format, va_list args)
{
    struct file *f;
    int rc=0;

    /* Get stdout file handle if any */
    if (in_atomic() || in_interrupt() || !current || !current->files)
        f = 0;
    else
        f = fcheck(STDOUT_FILENO);
    if (!f)
    {
         rc = vprintk(format, args);
    }
    else
    {

        char *printbuf=NULL;
        char *p1=NULL, *p2;
        p1=printbuf=kvasprintf(GFP_KERNEL, format, args);
        if(printbuf)
        {
            do
            {
                p2 = strchr(p1, '\n');
                if (p2)
                {
                    rc += _bdmf_write(f, (unsigned char*) p1, p2-p1+1);
                    rc += _bdmf_write(f, (unsigned char*)"\r", 1);
                }
                else
                    rc += _bdmf_write(f, (unsigned char*)p1, strlen(p1));
                //rc += p2 - p1 + 1;
                p1 = p2 + 1;
            } while(p2);
            kfree(printbuf);
        }
    }
    return rc;
}

int bdmf_print(const char *format, ...)
{
    int rc;
    va_list args;
    va_start(args, format);
    rc = bdmf_vprint(format, args);
    va_end(args);
    return rc;
}

/*
 * File IO
 */

bdmf_file bdmf_file_open(const char *fname, int flags)
{
    int oflags=0;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    struct file *fd;

    if ((flags & BDMF_FMODE_RDWR) == BDMF_FMODE_RDWR)
        oflags |= O_RDWR;
    else if ((flags & BDMF_FMODE_RDONLY))
        oflags |= O_RDONLY;
    else if ((flags & BDMF_FMODE_WRONLY))
        oflags |= O_WRONLY;

    if ((flags & BDMF_FMODE_CREATE))
        oflags |= O_CREAT;
    if ((flags & BDMF_FMODE_APPEND))
        oflags |= O_APPEND;
    if ((flags & BDMF_FMODE_TRUNCATE))
        oflags |= O_TRUNC;

    fd = filp_open(fname, oflags, mode);

    if (IS_ERR(fd))
        return NULL;

    return fd;
}

void bdmf_file_close(bdmf_file fd)
{
    filp_close(fd, 0);
}

int bdmf_file_read(bdmf_file fd, void *buf, uint32_t size)
{
    int n;

    n = kernel_read(fd, buf, size, &fd->f_pos);
    return ((n >= 0) ? n : BDMF_ERR_IO);
}

int bdmf_file_write(bdmf_file fd, const void *buf, uint32_t size)
{
    int n;

    n = kernel_write(fd, buf, size, &fd->f_pos);
    return ((n >= 0) ? n : BDMF_ERR_IO);
}

/*
 * Timer support
 */

static void _bdmf_timer_cb(struct timer_list *timer)
{
    bdmf_timer_t *bdmf_timer = from_timer(bdmf_timer, timer, timer);
    bdmf_timer->cb(bdmf_timer, bdmf_timer->priv);
}

void bdmf_timer_init(bdmf_timer_t *bdmf_timer, bdmf_timer_cb_t cb, unsigned long priv)
{
    bdmf_timer->cb = cb;
    bdmf_timer->priv = priv;
    timer_setup(&bdmf_timer->timer, _bdmf_timer_cb, 0);
}

/*
 * Interrupt control
 */

/** Disconnect system interrupt
 * \param[in]   irq     IRQ number
 * \param[in]   priv    Private cookie passed in bdmf_int_connect()
 * \returns 0=OK, <0- error
 */
void bdmf_int_disconnect(int irq, void *priv)
{
    free_irq(irq, priv);
}

/*
 * Queues
 */

struct bdmf_queue_message_t
{
    struct list_head messages_list;
    char *p_message_buffer;
    uint32_t message_length;			
};

bdmf_error_t bdmf_queue_create(bdmf_queue_t *queue, uint32_t number_of_messages, uint32_t max_message_length)
{
    bdmf_queue_message_t *p_structs_buffer = NULL;
    bdmf_queue_message_t *p_message_struct;
    char *p_messages_data_buffer = NULL;
    int32_t i;
    bdmf_error_t bdmf_error;

    memset(queue, 0, sizeof(*queue));
    INIT_LIST_HEAD(&queue->free_message_list);
    INIT_LIST_HEAD(&queue->full_message_list);
    bdmf_fastlock_init(&queue->lock);

    /* Allocate buffer for messages data */
    p_messages_data_buffer = bdmf_alloc(number_of_messages * max_message_length);
    if (!p_messages_data_buffer)
        return BDMF_ERR_NOMEM;

    /* allocate  all messages structs */
    p_structs_buffer = bdmf_alloc(number_of_messages * sizeof(bdmf_queue_message_t));
    if (!p_structs_buffer)
    {
        bdmf_error = BDMF_ERR_NOMEM;
        goto Error;
    }

    /* Create receive semaphore */
    sema_init(&queue->receive_waiting_semaphore, 0);

    bdmf_fastlock_lock(&queue->lock);

    /* Create queue */
    for (i = 0; i < number_of_messages; i++)
    {
        p_message_struct = &p_structs_buffer[i];
        /* Set all message pointers to actual buffers */
        p_message_struct->p_message_buffer = &p_messages_data_buffer[i * max_message_length];
        /* Add all messages to free list */
        list_add(&p_message_struct->messages_list, &queue->free_message_list);
    }

    /* Update queue struct */
    queue->number_of_free_messages = number_of_messages;
    queue->p_messages_data_buffer = p_messages_data_buffer;
    queue->p_structs_buffer = p_structs_buffer;

    bdmf_fastlock_unlock(&queue->lock);

    return 0;

Error:
    if (p_structs_buffer)
        bdmf_free(p_structs_buffer);
    if (p_messages_data_buffer)
        bdmf_free(p_messages_data_buffer);
    return bdmf_error;
}

bdmf_error_t bdmf_queue_delete(bdmf_queue_t *queue)
{
    int32_t i;
    void *p_messages_data_buffer;
    bdmf_queue_message_t *p_structs_buffer;

    if (!queue->free_message_list.next)
        return 0; /* queue is uninitialized */

    bdmf_fastlock_lock(&queue->lock);

    /* Release all free message structs */
    for (i = 0; i < queue->number_of_free_messages; i++)
    {
        if (list_empty(&queue->free_message_list))
            break;
        list_del_init(&queue->free_message_list);
    }

    /* Release all full message structs */
    for (i = 0; i < queue->number_of_messages; i++)
    {
        if (list_empty(&queue->full_message_list))
            break;
        list_del_init(&queue->full_message_list);
    }

    p_messages_data_buffer = queue->p_messages_data_buffer;
    p_structs_buffer = queue->p_structs_buffer;

    bdmf_fastlock_unlock(&queue->lock);

    /* Free allocated memory */
    if (p_messages_data_buffer) 
        bdmf_free(p_messages_data_buffer);

    if (p_structs_buffer)
        bdmf_free(p_structs_buffer);

    return 0;
}

bdmf_error_t bdmf_queue_send(bdmf_queue_t *queue, char *buffer, uint32_t length)
{
    bdmf_error_t bdmf_error;
    bdmf_queue_message_t *p_message_struct;

    bdmf_fastlock_lock(&queue->lock);

    if (!queue->number_of_free_messages)
    {
        bdmf_error = BDMF_ERR_NORES; /* No resources error */
        goto Exit;
    }

    /* Update full & free lists */
    p_message_struct = list_entry(queue->free_message_list.next, bdmf_queue_message_t, messages_list);

    /* Remove from empty list */
    list_del_init(&p_message_struct->messages_list); 								
    /* Add to full list end */
    list_add_tail(&p_message_struct->messages_list, &queue->full_message_list);		

    /* Copy Data from User to queue buffer */
    memcpy(p_message_struct->p_message_buffer, buffer, length);
    p_message_struct->message_length = length;

    /* Free any tasks waiting for messages */
    up(&queue->receive_waiting_semaphore);

    /* Upadte message counters */
    queue->number_of_free_messages--;
    queue->number_of_messages++;

    bdmf_error = 0;

Exit:
    bdmf_fastlock_unlock(&queue->lock);
    return bdmf_error;
}

bdmf_error_t bdmf_queue_receive(bdmf_queue_t *queue, char *buffer, uint32_t *length)
{
    bdmf_queue_message_t *p_message_struct;

    /* Taking counting semaphore to make sure we are not accessing an empty queue*/
    down(&queue->receive_waiting_semaphore);

    bdmf_fastlock_lock(&queue->lock);

    /* Update full & free lists */
    p_message_struct = list_entry(queue->full_message_list.next, bdmf_queue_message_t, messages_list);

    list_del_init(&p_message_struct->messages_list); /* Remove from full list */
    list_add_tail(&p_message_struct->messages_list, &queue->free_message_list); /* Add to free list */

    memcpy(buffer, p_message_struct->p_message_buffer, p_message_struct->message_length);

    /* Upadte message counters */
    queue->number_of_free_messages++;
    queue->number_of_messages--;

    /* Return the number of bytes actually read */
    *length = p_message_struct->message_length;

    bdmf_fastlock_unlock(&queue->lock);

    return 0;
}

/*
 * Semaphores
 */

void bdmf_semaphore_create(bdmf_semaphore_t *semaphore, int val)
{
    sema_init(semaphore, val);
}

void bdmf_semaphore_give(bdmf_semaphore_t *sem)
{
    up(sem);
}

void bdmf_semaphore_take(bdmf_semaphore_t *sem)
{
    down(sem);
}

/*
 * Exports
 */

EXPORT_SYMBOL(bdmf_alloc);
EXPORT_SYMBOL(bdmf_calloc);
EXPORT_SYMBOL(bdmf_free);
EXPORT_SYMBOL(bdmf_alloc_uncached);
EXPORT_SYMBOL(bdmf_free_uncached);
EXPORT_SYMBOL(bdmf_task_create);
EXPORT_SYMBOL(bdmf_task_destroy);
EXPORT_SYMBOL(bdmf_task_get_name);
EXPORT_SYMBOL(bdmf_task_get_current);
EXPORT_SYMBOL(bdmf_mutex_init);
EXPORT_SYMBOL(bdmf_mutex_lock);
EXPORT_SYMBOL(bdmf_mutex_unlock);
EXPORT_SYMBOL(bdmf_ta_mutex_init);
EXPORT_SYMBOL(bdmf_ta_mutex_lock);
EXPORT_SYMBOL(bdmf_ta_mutex_unlock);
EXPORT_SYMBOL(bdmf_ta_mutex_delete);
EXPORT_SYMBOL(bdmf_reent_fastlock_init);
EXPORT_SYMBOL(bdmf_reent_fastlock_lock);
EXPORT_SYMBOL(bdmf_reent_fastlock_unlock);
EXPORT_SYMBOL(bdmf_vprint);
EXPORT_SYMBOL(bdmf_print);
EXPORT_SYMBOL(bdmf_file_open);
EXPORT_SYMBOL(bdmf_file_close);
EXPORT_SYMBOL(bdmf_file_read);
EXPORT_SYMBOL(bdmf_file_write);
EXPORT_SYMBOL(bdmf_timer_init);
EXPORT_SYMBOL(bdmf_int_disconnect);
EXPORT_SYMBOL(bdmf_queue_create);
EXPORT_SYMBOL(bdmf_queue_delete);
EXPORT_SYMBOL(bdmf_queue_send);
EXPORT_SYMBOL(bdmf_queue_receive);
EXPORT_SYMBOL(bdmf_semaphore_create);
EXPORT_SYMBOL(bdmf_semaphore_give);
EXPORT_SYMBOL(bdmf_semaphore_take);
