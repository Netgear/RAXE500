/*
 * Copyright (c) 2009 Mark Heily <mark@heily.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/queue.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include <limits.h>
#include <sys/inotify.h>
#include <sys/epoll.h>

#include "sys/event.h"
#include "private.h"

static char *
inotify_mask_dump(uint32_t mask)
{
    static char __thread buf[1024];

#define INEVT_MASK_DUMP(attrib) \
    if (mask & attrib) \
       strcat(buf, #attrib" ");

    snprintf(buf, sizeof(buf), "mask = %d (", mask);
    INEVT_MASK_DUMP(IN_ACCESS);
    INEVT_MASK_DUMP(IN_MODIFY);
    INEVT_MASK_DUMP(IN_ATTRIB);
    INEVT_MASK_DUMP(IN_CLOSE_WRITE);
    INEVT_MASK_DUMP(IN_CLOSE_NOWRITE);
    INEVT_MASK_DUMP(IN_OPEN);
    INEVT_MASK_DUMP(IN_MOVED_FROM);
    INEVT_MASK_DUMP(IN_MOVED_TO);
    INEVT_MASK_DUMP(IN_CREATE);
    INEVT_MASK_DUMP(IN_DELETE);
    INEVT_MASK_DUMP(IN_DELETE_SELF);
    INEVT_MASK_DUMP(IN_MOVE_SELF);
    buf[strlen(buf) - 1] = ')';

    return (buf);
}

static char *
inotify_event_dump(struct inotify_event *evt)
{
    static char __thread buf[1024];

    snprintf(buf, sizeof(buf), "wd=%d mask=%s", 
            evt->wd,
            inotify_mask_dump(evt->mask));

    return (buf);
}

static int
fd_to_path(char *buf, size_t bufsz, int fd)
{
    char path[1024];    //TODO: Maxpathlen, etc.

    if (snprintf(&path[0], sizeof(path), "/proc/%d/fd/%d", getpid(), fd) < 0)
        return (-1);

    memset(buf, 0, bufsz);
    return (readlink(path, buf, bufsz));
}


/* TODO: USE this to get events with name field */
int
get_one_event(struct inotify_event *dst, int pfd)
{
    ssize_t n;

    dbg_puts("reading one inotify event");
    for (;;) {
        n = read(pfd, dst, sizeof(*dst));
        if (n < 0) {
            if (errno == EINTR)
                continue;
            dbg_perror("read");
            return (-1);
        } else {
            break;
        }
    }
    dbg_printf("read(2) from inotify wd: %zu bytes", n);

    /* FIXME-TODO: if len > 0, read(len) */
    if (dst->len != 0) 
        abort();


    return (0);
}

static int
add_watch(struct filter *filt, struct knote *kn)
{
    char path[PATH_MAX];
    uint32_t mask;

    /* Convert the fd to a pathname */
    if (fd_to_path(&path[0], sizeof(path), kn->kev.ident) < 0)
        return (-1);

    /* Convert the fflags to the inotify mask */
    mask = 0;
    if (kn->kev.fflags & NOTE_DELETE)
        mask |= IN_ATTRIB | IN_DELETE_SELF;
    if (kn->kev.fflags & NOTE_WRITE)      
        mask |= IN_MODIFY | IN_ATTRIB;
    if (kn->kev.fflags & NOTE_EXTEND)
        mask |= IN_MODIFY | IN_ATTRIB;
    if ((kn->kev.fflags & NOTE_ATTRIB) || 
            (kn->kev.fflags & NOTE_LINK))
        mask |= IN_ATTRIB;
    if (kn->kev.fflags & NOTE_RENAME)
        mask |= IN_MOVE_SELF;
    if (kn->kev.flags & EV_ONESHOT)
        mask |= IN_ONESHOT;

    dbg_printf("inotify_add_watch(2); inofd=%d, %s, path=%s", 
            filt->kf_pfd, inotify_mask_dump(mask), path);
    kn->kev.data = inotify_add_watch(filt->kf_pfd, path, mask);
    if (kn->kev.data < 0) {
        dbg_printf("inotify_add_watch(2): %s", strerror(errno));
        return (-1);
    }
    return (0);
}

static int
delete_watch(struct filter *filt, struct knote *kn)
{
    if (kn->kev.data < 0)
        return (0);
    if (inotify_rm_watch(filt->kf_pfd, kn->kev.data) < 0) {
        dbg_printf("inotify_rm_watch(2): %s", strerror(errno));
        return (-1);
    }
    dbg_printf("wd %d removed", (int) kn->kev.data);
    kn->kev.data = -1;

    return (0);
}

int
evfilt_vnode_init(struct filter *filt)
{
    filt->kf_pfd = inotify_init();
    dbg_printf("inotify fd = %d", filt->kf_pfd);
    if (filt->kf_pfd < 0)
        return (-1);

    return (0);
}

void
evfilt_vnode_destroy(struct filter *filt)
{
    close(filt->kf_pfd);
}

int
evfilt_vnode_copyout(struct filter *filt, 
            struct kevent *dst, 
            int nevents)
{
    struct inotify_event evt;
    struct stat sb;
    struct knote *kn;

    if (get_one_event(&evt, filt->kf_pfd) < 0)
        return (-1);

    dbg_printf("inotify event: %s", inotify_event_dump(&evt));
    if (evt.mask & IN_IGNORED) {
        /* TODO: possibly return error when fs is unmounted */
        return (0);
    }

    kn = knote_lookup_data(filt, evt.wd);
    if (kn == NULL) {
        dbg_printf("no match for wd # %d", evt.wd);
        return (-1);
    }

    memcpy(dst, &kn->kev, sizeof(*dst));
    dst->data = 0;

    /* No error checking because fstat(2) should rarely fail */
    //FIXME: EINTR
    if ((evt.mask & IN_ATTRIB || evt.mask & IN_MODIFY) 
        && fstat(kn->kev.ident, &sb) == 0) {
        if (sb.st_nlink == 0 && kn->kev.fflags & NOTE_DELETE) 
            dst->fflags |= NOTE_DELETE;
        if (sb.st_nlink != kn->data.vnode.nlink && kn->kev.fflags & NOTE_LINK) 
            dst->fflags |= NOTE_LINK;
#if HAVE_NOTE_TRUNCATE
        if (sb.st_nsize == 0 && kn->kev.fflags & NOTE_TRUNCATE) 
            dst->fflags |= NOTE_TRUNCATE;
#endif
        if (sb.st_size > kn->data.vnode.size && kn->kev.fflags & NOTE_WRITE) 
            dst->fflags |= NOTE_EXTEND;
       kn->data.vnode.nlink = sb.st_nlink;
       kn->data.vnode.size = sb.st_size;
    }

    if (evt.mask & IN_MODIFY && kn->kev.fflags & NOTE_WRITE) 
        dst->fflags |= NOTE_WRITE;
    if (evt.mask & IN_ATTRIB && kn->kev.fflags & NOTE_ATTRIB) 
        dst->fflags |= NOTE_ATTRIB;
    if (evt.mask & IN_MOVE_SELF && kn->kev.fflags & NOTE_RENAME) 
        dst->fflags |= NOTE_RENAME;
    if (evt.mask & IN_DELETE_SELF && kn->kev.fflags & NOTE_DELETE) 
        dst->fflags |= NOTE_DELETE;

    if (evt.mask & IN_MODIFY && kn->kev.fflags & NOTE_WRITE) 
        dst->fflags |= NOTE_WRITE;
    if (evt.mask & IN_ATTRIB && kn->kev.fflags & NOTE_ATTRIB) 
        dst->fflags |= NOTE_ATTRIB;
    if (evt.mask & IN_MOVE_SELF && kn->kev.fflags & NOTE_RENAME) 
        dst->fflags |= NOTE_RENAME;
    if (evt.mask & IN_DELETE_SELF && kn->kev.fflags & NOTE_DELETE) 
        dst->fflags |= NOTE_DELETE;

    if (kn->kev.flags & EV_DISPATCH) {
        delete_watch(filt, kn); /* TODO: error checking */
        KNOTE_DISABLE(kn);
    } else if (kn->kev.flags & EV_ONESHOT) {
        delete_watch(filt, kn); /* TODO: error checking */
        knote_free(filt, kn);
    }
            
    return (1);
}

int
evfilt_vnode_knote_create(struct filter *filt, struct knote *kn)
{
    struct stat sb;

    if (fstat(kn->kev.ident, &sb) < 0) {
        dbg_puts("fstat failed");
        return (-1);
    }
    kn->data.vnode.nlink = sb.st_nlink;
    kn->data.vnode.size = sb.st_size;
    kn->kev.data = -1;

    return (add_watch(filt, kn));
}

int
evfilt_vnode_knote_modify(struct filter *filt, struct knote *kn, 
        const struct kevent *kev)
{
    return (-1); /* FIXME - STUB */
}

int
evfilt_vnode_knote_delete(struct filter *filt, struct knote *kn)
{   
    return delete_watch(filt, kn);
}

int
evfilt_vnode_knote_enable(struct filter *filt, struct knote *kn)
{
    return add_watch(filt, kn);
}

int
evfilt_vnode_knote_disable(struct filter *filt, struct knote *kn)
{
    return delete_watch(filt, kn);
}

const struct filter evfilt_vnode = {
    EVFILT_VNODE,
    evfilt_vnode_init,
    evfilt_vnode_destroy,
    evfilt_vnode_copyout,
    evfilt_vnode_knote_create,
    evfilt_vnode_knote_modify,
    evfilt_vnode_knote_delete,
    evfilt_vnode_knote_enable,
    evfilt_vnode_knote_disable,        
};
