/* SPDX-License-Identifier: GPL-2.0+
*  *
*   *  Copyright 2019 Broadcom Ltd.
*    */

#include <clk.h>
#include <common.h>
#include <dm.h>
#include <net.h>
#include <config.h>
#include <console.h>
#include <malloc.h>
#include <asm/io.h>
#include <phy.h>
#include <miiphy.h>
#include <wait_bit.h>
#include <watchdog.h>
#include <asm/system.h>
#include <linux/errno.h>
#include <linux/io.h>
#include <linux/ioport.h>

#include "bcm_ethsw.h"
#include "bcmbca_sysport_v2.h"

#define MAX_PKT_LEN                             (NET_DMA_BUFSIZE)
enum sysport_ids
{
	SYSPORT_0,
	SYSPORT_1,
};

#define MAX_ETHSW   2

struct sysport_priv
{
	int init;
	int active_sysport;
	unsigned char *buffers[2];
	sys_port_rbuf *sys_port_rbuf[2];
	sys_port_rdma *sys_port_rdma[2];
	sys_port_tdma *sys_port_tdma[2];
	sys_port_umac *sys_port_umac[2];
	sys_port_gib    *sys_port_gib;
	sys_port_topctrl *sys_port_topctrl[2];

	void		*switch_mdio;
	void		*sphy_ctrl;
	void		*phy_test_ctrl;
	u32 		phy_wkard_timeout;
	int 		phy_base;

	struct udevice *ethsw_devp[MAX_ETHSW];
	bcm_ethsw_ops_t *ethsw_ops[MAX_ETHSW];
};

char static_buff[2048];


static void init_pkt_desc(volatile void *ptr, unsigned char *rx_dma_bufaddr)
{
	int i;
	volatile PktDesc *pkt_desc=(PktDesc *)ptr;

	for(i=0;i<(1<<SYSPORT_NUM_RX_PKT_DESC_LOG2);i++)
	{
		pkt_desc[i].address=(uintptr_t)rx_dma_bufaddr+(2048*i);
		pkt_desc[i].address_hi=0;
		pkt_desc[i].status=0;
		pkt_desc[i].length=2048;
	}
}

/* Enable System Port TX DMA */
static int __sp_enable_tdma(volatile sys_port_tdma *sysport_tdma)
{
	volatile uint32_t reg;
	int timeout = 1000;

	// Disable ACB and enable TDMA
	sysport_tdma->SYSTEMPORT_TDMA_CONTROL |= 0x01;

	// Wait for TX DMA to become ready
	do {
		reg = sysport_tdma->SYSTEMPORT_TDMA_STATUS;
		if (!(reg & 0x3)) {
			return 0;
		}
		udelay(10);

	} while (timeout--);

	return -1;
}/* Enable System Port RX DMA */


/* Enable System Port TX DMA */
static int sysport_enable_tdma(struct sysport_priv *priv)
{
	int init_0 =0, init_1=0;

	if(priv->sys_port_tdma[0] != NULL)
		init_0=__sp_enable_tdma(priv->sys_port_tdma[0]);
	if(priv->sys_port_tdma[1] != NULL)
		init_1=__sp_enable_tdma(priv->sys_port_tdma[1]);
	if(init_0 != 0 || init_0 != 0)
	{
		init_0=-1;
		printf("enble tdma failed [0] %d [1] %d\n", init_0, init_1);
	}
	return init_0;
}

static int __sp_enable_rdma(volatile sys_port_rdma *sysport_rdma)
{
	volatile uint32_t reg;
	int timeout = 1000;

	// Enable RX DMA and Ring Buffer mode
	sysport_rdma->SYSTEMPORT_RDMA_CONTROL |= 0x01;

	// Wait for RX DMA to become ready
	do {
		reg = sysport_rdma->SYSTEMPORT_RDMA_STATUS;
		if (!(reg & 0x3)) {
			return 0;
		}
		udelay(10);

	} while (timeout--);

	return -1;
}

/* Enable System Port RX DMA */
static int sysport_enable_rdma(struct sysport_priv *priv)
{

	int init_0 =0, init_1=0;

	if(priv->sys_port_rdma[0] != NULL)
		init_0=__sp_enable_rdma(priv->sys_port_rdma[0]);

	if(priv->sys_port_rdma[1] != NULL)
		init_1=__sp_enable_rdma(priv->sys_port_rdma[1]);
	if(init_0 != 0 || init_0 != 0)
	{
		init_0=-1;
		printf("enble rdma failed [0] %d [1] %d\n", init_0, init_1);
	}
	return init_0;
}

static int __sp_init(volatile sys_port_rbuf *sysport_rbuf, volatile sys_port_rdma *sysport_rdma, volatile sys_port_tdma *sysport_tdma, unsigned char *dma_buffer_addr)
{

	/* all memory allocation are done already */
	uint32_t v32 = 0;

	/* System Port RBUF configuration */
	v32 = sysport_rbuf->SYSTEMPORT_RBUF_RBUF_CONTROL;     /* Read Chip Defaults */
	v32 &= ~SYSPORT_RBUF_CTRL_RSB_MODE_M;                   /* Disable RSB */
	v32 &= ~SYSPORT_RBUF_CTRL_4B_ALIGN_M;                 /* Disable 4-Byte IP Alignment */
	v32 &= ~SYSPORT_RBUF_CTRL_BTAG_STRIP_M;               /* Do not strip BRCM TAG */
	v32 |= SYSPORT_RBUF_CTRL_BAD_PKT_DISCARD_M;           /* Discard Bad Packets */
	/* Read-Modify-Write */
	sysport_rbuf->SYSTEMPORT_RBUF_RBUF_CONTROL=v32; 

	sysport_rbuf->SYSTEMPORT_RBUF_RBUF_PACKET_READY_THRESHOLD=0x80; /* Keep chip default */

	/* System Port TBUF configuration -- No change, keep chip defaults */

	/* System Port RDMA Configuration */

/*since default has the correct values, and there is bit variation between 63158/178 and 47622, keep this unchanged */
#if 0
	/* RDMA Control Register */
	v32 = sysport_rdma->SYSTEMPORT_RDMA_CONTROL;  /* Read Chip Defaults */
	v32 &= ~SYSPORT_RDMA_CTRL_RDMA_EN_M;          /* Disable RDMA */
	v32 &= ~SYSPORT_RDMA_CTRL_RING_CFG_bit1_M;          /* Enable Descriptor Ring Mode */
	v32 |= SYSPORT_RDMA_CTRL_DISCARD_EN_M;        /* Enable Pkt discard by RDMA when ring full */
	v32 &= ~SYSPORT_RDMA_CTRL_DATA_OFFSET_M;       /* Zero data offset - this feature could be used later to reduce host buffer size */
	v32 &= ~SYSPORT_RDMA_CTRL_DDR_DESC_WR_EN_M;
//	v32 |= SYSPORT_RDMA_CTRL_DDR_DESC_SWAP_M;     /* Both Byte and word swap enabled for Desc - TBD - need to understand */
	/* Read-Modify-Write */
	sysport_rdma->SYSTEMPORT_RDMA_CONTROL=v32; 
#endif

	/* RDMA Buffer and Ring Size Register */
	//v32 = 0;/* Reset register  */
	//v32 |= ( (SYSPORT_PKT_LEN_LOG2 << SYSPORT_RDMA_BSRS_BUF_SIZE_LOG2_S) & SYSPORT_RDMA_BSRS_BUF_SIZE_LOG2_M ); /* set buf size */
	//v32 |= ( ((1<<SYSPORT_NUM_RX_PKT_DESC_LOG2) << SYSPORT_RDMA_BSRS_RING_SIZE_S) & SYSPORT_RDMA_BSRS_RING_SIZE_M ); /* force chip default of 512 */
	sysport_rdma->SYSTEMPORT_RDMA_LOCRAM_DESCRING_SIZE[0]=SYSPORT_RDMA_RING_EN | (1<<SYSPORT_NUM_RX_PKT_DESC_LOG2);
	sysport_rdma->SYSTEMPORT_RDMA_PKTBUF_SIZE[0]=SYSPORT_PKT_LEN_LOG2;


	/* RDMA Consumer Index Register */
	/* Initialize RX DMA consumer index - low 16 bit; High 16-bits are read-only */
	sysport_rdma->SYSTEMPORT_RDMA_CINDEX[0]=0x0;
	sysport_rdma->SYSTEMPORT_RDMA_PINDEX[0]=0x0;

	/* RDMA Desc Start Address Registers */
	/* In desciptor ring mode - start address is index = 0 */
	sysport_rdma->SYSTEMPORT_RDMA_DDR_DESC_RING_START[0]=0;
	sysport_rdma->SYSTEMPORT_RDMA_DDR_DESC_RING_START[1]=0;


	init_pkt_desc(sysport_rdma->SYSTEMPORT_RDMA_DESCRIPTOR, dma_buffer_addr);

	/* RDMA DDR Desc Ring Register */
	//sysport_rdma->SYSTEMPORT_RDMA_DDR_DESC_RING_START_LOW = 0;
	//sysport_rdma->SYSTEMPORT_RDMA_DDR_DESC_RING_START_HIGH = 0; /* Ideally we should put the Hi 8-bits here */

	/* RDMA DDR Desc Ring Size Register */
	sysport_rdma->SYSTEMPORT_RDMA_DDR_DESC_RING_SIZE[0] = SYSPORT_NUM_RX_PKT_DESC_LOG2;

	/* RDMA Multi-Buffer-Done-Interrupt-Threshold : No timeout & interrupt every packet */
	//sysport_rdma->SYSTEMPORT_RDMA_MULTIPLE_BUFFERS_DONE_INTERRUPT_THRESHOLD_PUSH_TIMER = 1;
	/* enable DDR DESC write push timer to 1 timer tick (equals 1024 RDMA sys clocks */
	//sysport_rdma->SYSTEMPORT_RDMA_DDR_DESC_RING_WR_PUSH_TIMER[0] = (0x1 & SYSTEMPORT_RDMA_DDR_DESC_RING_PUSH_TIMER_TIMEOUT_M);
	/* TDMA Block Configuration */

	/* System port supports upto 16 Desc Rings;
	Only one TX DDR Desc ring is used; It is mapped to TX-Queue[0] */

	/* Enable TX Q#0 */
	//sysport_tdma->SYSTEMPORT_TDMA_DESC_RING_HEAD_TAIL_PTR[0] = SYSPORT_TDMA_DESC_RING_XX_HEAD_TAIL_PTR_RING_EN_M;

	sysport_tdma->SYSTEMPORT_TDMA_DESC_RING_CONTROL[0]=SYSTEMPORT_TDMA_DESC_RING_CONTROL_RING_EN;
	/* Initialize Producer & Consumer Index */
	sysport_tdma->SYSTEMPORT_TDMA_DESC_RING_PRODUCER_CONSUMER_INDEX[0] = 0;
	/* Q#0 DDR Desc Ring Address */
	sysport_tdma->SYSTEMPORT_TDMA_DDR_DESC_RING_START[0] = 0;
	sysport_tdma->SYSTEMPORT_TDMA_DDR_DESC_RING_START[1] = 0; /* Ideally this should be high 8-bit of address */

	sysport_tdma->SYSTEMPORT_TDMA_DESC_RING_MAPPING[0] = 0x40;
	/* enable DDR DESC read push timer to 1 timer tick (equals 1024 TDMA sys clocks */
	sysport_tdma->SYSTEMPORT_TDMA_DDR_DESC_RING_PUSH_TIMER[0] = (0x1 & SYSTEMPORT_TDMA_DDR_DESC_RING_PUSH_TIMER_TIMEOUT_M);
	/* Q#0 DDR Desc Ring Size Log2 */
	sysport_tdma->SYSTEMPORT_TDMA_DDR_DESC_RING_SIZE[0] = SYSPORT_NUM_TX_PKT_DESC_LOG2;
	sysport_tdma->SYSTEMPORT_TDMA_DESC_RING_INTR_CONTROL[0] = 0x3;
	sysport_tdma->SYSTEMPORT_TDMA_DESC_RING_MAX_THRESHOLD[0] = 0x00100009;
	/* enable arbitrator for Q#0 */
	sysport_tdma->SYSTEMPORT_TDMA_TIER2_ARBITER_CTRL = 0x1; /* Round Robin */
	sysport_tdma->SYSTEMPORT_TDMA_TIER1_ARBITER_CTRL[0] = 0x1;
	sysport_tdma->SYSTEMPORT_TDMA_TIER1_ARBITER_CTRL[1] = 0x1;
	sysport_tdma->SYSTEMPORT_TDMA_TIER1_ARBITER_CTRL[2] = 0x1;
	sysport_tdma->SYSTEMPORT_TDMA_TIER1_ARBITER_CTRL[3] = 0x1;
	sysport_tdma->SYSTEMPORT_TDMA_TIER1_ARBITER_QUEUE_ENABLE[0] = 0x000000ff;
	sysport_tdma->SYSTEMPORT_TDMA_TIER1_ARBITER_QUEUE_ENABLE[1] = 0x0000ff00;
	sysport_tdma->SYSTEMPORT_TDMA_TIER1_ARBITER_QUEUE_ENABLE[2] = 0x00ff0000;
	sysport_tdma->SYSTEMPORT_TDMA_TIER1_ARBITER_QUEUE_ENABLE[3] = 0xff000000;
	/* TDMA Control Register */
	v32 = sysport_tdma->SYSTEMPORT_TDMA_CONTROL; /* Read chip defaults */
	v32 &= ~SYSPORT_TDMA_CONTROL_TSB_EN_M; /* Disable TSB */
	v32 &= ~(SYSPORT_TDMA_CONTROL_RING_CFG_M); /* Disable DDR Desc Ring fetch */
	v32 &= ~(SYSPORT_TDMA_CONTROL_ACB_EN_M); /* No ACB */
	sysport_tdma->SYSTEMPORT_TDMA_CONTROL = v32;

	/* Enable Tier-1 arbiter for Q#0 */
	sysport_tdma->SYSTEMPORT_TDMA_TIER1_ARBITER_QUEUE_ENABLE[0]=0x1;


	return 0;
}


static int sysport_init(struct udevice *dev)
{
	struct sysport_priv *priv = dev_get_priv(dev);
	uint32_t v32;
	int i;

	if (!priv->init)
	{
		for (i = 0; i < MAX_ETHSW; i++)
    		if (priv->ethsw_devp[i])
    			priv->ethsw_ops[i]->open (priv->ethsw_devp[i]);

		/* all memory allocation are done already */
		if (priv->sys_port_rbuf[0] != NULL) {
			__sp_init(priv->sys_port_rbuf[0], priv->sys_port_rdma[0], priv->sys_port_tdma[0], priv->buffers[0]); 
		}
		if (priv->sys_port_rbuf[1] != NULL) {
			__sp_init(priv->sys_port_rbuf[1], priv->sys_port_rdma[1], priv->sys_port_tdma[1], priv->buffers[1]); 
		}

		if(priv->sys_port_gib != NULL)
		{
			v32=priv->sys_port_gib->SYSTEMPORT_GIB_CONTROL;
			v32 |= SYSPORT_GIB_CONTROL_TX_EN | SYSPORT_GIB_CONTROL_RX_EN;
			priv->sys_port_gib->SYSTEMPORT_GIB_CONTROL = v32;
		}

		// Enable RX DMA
		if (sysport_enable_rdma(priv) < 0) {
			// Print SPIF for System Port Init Failed

			return -1;
		}

		// Enable TX DMA
		if (sysport_enable_tdma(priv) < 0) {
			// Print SPIF for System Port Init Failed

			return -1;
		}

		if(priv->sys_port_umac[0] != NULL)
		{
			v32 = priv->sys_port_umac[0]->SYSTEMPORT_UMAC_CMD;
			v32 |= SYSPORT_UNIMAC_COMMAND_CONFIG_TX_EN; /* Enable TX */
			v32 |= SYSPORT_UNIMAC_COMMAND_CONFIG_RX_EN; /* Enable RX */
			priv->sys_port_umac[0]->SYSTEMPORT_UMAC_CMD=v32;
		}
		if(priv->sys_port_umac[1] != NULL)
		{
			v32 = priv->sys_port_umac[1]->SYSTEMPORT_UMAC_CMD;
			v32 |= SYSPORT_UNIMAC_COMMAND_CONFIG_TX_EN; /* Enable TX */
			v32 |= SYSPORT_UNIMAC_COMMAND_CONFIG_RX_EN; /* Enable RX */
			priv->sys_port_umac[1]->SYSTEMPORT_UMAC_CMD=v32;
		}

		// Give SP some time to initialize
		udelay(100);


			
	}
	priv->init++;
	return 0;
}


static int __sp_send(volatile sys_port_tdma *sysport_tdma, unsigned char* tx_dma_bufaddr, uint8_t *buffer, uint32_t length)
{
	int timeout = 1000;
	uint32_t p_index, c_index;
	uint32_t txdesc_hi, txdesc_lo;

	if (length > MAX_PKT_LEN) {
		// Print SPWE for SP Write Error

		return -1;
	}

	memcpy((void *)(tx_dma_bufaddr), buffer, length);
	flush_cache((unsigned long)tx_dma_bufaddr, NET_DMA_BUFSIZE);

	// set up txdesc record: eop=1, sop=1, append_crc=1
	txdesc_hi = (length << 18) | (3 << 16) | (1 << 11);
	txdesc_lo = (uint32_t)(tx_dma_bufaddr);

	// write tx desc to hw
	// SYSTEMPORT_TDMA_DESCRIPTOR_00_WRITE_PORT_LO
	sysport_tdma->SYSTEMPORT_TDMA_DESCRIPTOR_WRITE_PORT[0].SYSTEMPORT_TDMA_DESCRIPTOR_XX_WRITE_PORT_LO=txdesc_lo;

        barrier();

	// SYSTEMPORT_TDMA_DESCRIPTOR_00_WRITE_PORT_HI
	sysport_tdma->SYSTEMPORT_TDMA_DESCRIPTOR_WRITE_PORT[0].SYSTEMPORT_TDMA_DESCRIPTOR_XX_WRITE_PORT_HI=txdesc_hi;

	// wait until the packet is fully DMA'ed and stored into TBUF
	do {
		// SYSTEMPORT_TDMA_DESC_RING_00_PRODUCER_CONSUMER_INDEX
		p_index = sysport_tdma->SYSTEMPORT_TDMA_DESC_RING_PRODUCER_CONSUMER_INDEX[0];
		c_index = (p_index >> 16);
		p_index &= 0xffff;
		c_index &= 0xffff;

	} while ((p_index != c_index) && timeout--);

	if (timeout == 0) {
		// Print SPWF for SP Write Failed

		return -1;
	}

	return 0;
}

static int sysport_send(struct udevice *dev, void *buffer, int length)
{
	struct sysport_priv *priv=(struct sysport_priv*)dev->priv;

	if(priv->active_sysport==SYSPORT_0)
		__sp_send(priv->sys_port_tdma[0], priv->buffers[0], buffer, length);
	if((priv->active_sysport==SYSPORT_1) && (priv->sys_port_tdma[1]))
		__sp_send(priv->sys_port_tdma[1], priv->buffers[1], buffer, length);
	return 0;


}

static int __sp_poll(volatile sys_port_rdma *sysport_rdma)
{
	uint32_t p_index, c_index;

	// SYSTEMPORT_RDMA_PRODUCER_INDEX...
	p_index = sysport_rdma->SYSTEMPORT_RDMA_PINDEX[0] & 0xffff;
	c_index = sysport_rdma->SYSTEMPORT_RDMA_CINDEX[0] & 0xffff;

	return (p_index == c_index);
}

static int sysport_poll(struct sysport_priv *priv, uint32_t timeout_ms)
{
	int i=0;
	while (1) {

		if(priv->sys_port_rdma[0] != NULL && __sp_poll(priv->sys_port_rdma[0]) == 0)
		{
			priv->active_sysport=SYSPORT_0;
			return 0;
		}
 		else if(priv->sys_port_rdma[1] != NULL && __sp_poll(priv->sys_port_rdma[1]) == 0)
		{
			priv->active_sysport=SYSPORT_1;
			return 0;
		}
		if (i++ > timeout_ms)
		{
			return -1;
		}
		udelay(500);
	}
	return 0;
}

int __sp_recv(volatile sys_port_rdma *sysport_rdma, uint8_t *buffer, uint32_t *length)
{
	int rc = 0;
	unsigned short rx_read_idx;
	const uint8_t *bufaddr;
	volatile PktDesc *pkt_desc;

	rx_read_idx=sysport_rdma->SYSTEMPORT_RDMA_CINDEX[0];
	rx_read_idx=rx_read_idx%NUM_RX_DMA_BUFFERS;
	// Get RX DMA buffer address
	pkt_desc=(PktDesc *)sysport_rdma->SYSTEMPORT_RDMA_DESCRIPTOR;

	flush_cache(pkt_desc[rx_read_idx].address, NET_DMA_BUFSIZE);
	bufaddr = (uint8_t *)((const uint8_t *)NULL + pkt_desc[rx_read_idx].address);

	*length=pkt_desc[rx_read_idx].length;


	if (*length < ENET_ZLEN || *length > MAX_PKT_LEN) {
		// Print SPRE for System Port Read Error

		rc = -1;
		goto out;
	}

	memcpy(buffer, bufaddr, *length);
        barrier();

out:
	sysport_rdma->SYSTEMPORT_RDMA_CINDEX[0]+=1;

	return rc;
}



static int sysport_recv(struct udevice *dev, int flags, uchar **packetp)
{
	struct sysport_priv *priv = dev_get_priv(dev);
	int rc = -1;
	int length=0;

	if(sysport_poll(priv, 2) == 0)
	{
		*packetp=(unsigned char *) static_buff;//memalign(sizeof(int), MAX_PKT_LEN);
		if (*packetp == NULL) {
			printf("%s: malloc failed\n", __func__);
			rc = -ENOMEM;
		} else {
			if(priv->sys_port_rdma[0] != NULL && priv->active_sysport == SYSPORT_0)
			{
				__sp_recv(priv->sys_port_rdma[0], *packetp, &length);
			}
			else if(priv->sys_port_rdma[1] != NULL) 
			{
				__sp_recv(priv->sys_port_rdma[1], *packetp, &length);
			}
			rc = length;
		}
	}
	return rc;
}

static int sysport_free_pkt(struct udevice *dev, uchar *packet, int length)
{
	if(packet != NULL && packet != static_buff)
		free(packet);
	return 0;
}

static void sysport_halt(struct udevice *dev)
{
}

static int sysport_remove(struct udevice *dev)
{
	struct sysport_priv *priv = dev_get_priv(dev);
	int i;

	printf("remove sysport\n");
	{
	    for (i = 0; i < MAX_ETHSW; i++)
    		if (priv->ethsw_devp[i])
    			priv->ethsw_ops[i]->close (priv->ethsw_devp[i]);

		sysport_reset(priv, 0, 1);

		if(priv->sys_port_rdma[1] != NULL) 
			sysport_reset(priv, 1, 1);
	}

	return 0;
}


static int sysport_read_rom_mac(struct udevice *dev)
{
	struct eth_pdata *pdata = dev_get_platdata(dev);
	u8 mac_addr[6]={0x02,0x10,0x00,0x00,0x00,0x01};
        u8 mac_temp[6];

	if (!eth_env_get_enetaddr("ethaddr", mac_temp))
	{
		eth_env_set_enetaddr("ethaddr", mac_addr);
	}
	return 0;
}

static const struct eth_ops sysport_ops = {
	.start			= sysport_init,
	.send			= sysport_send,
	.recv			= sysport_recv,
	.free_pkt		= sysport_free_pkt,
	.stop			= sysport_halt,
//	.write_hwaddr		= sysport_setup_mac,
	.read_rom_hwaddr	= sysport_read_rom_mac,
};




void sysport_reset(struct sysport_priv *priv, int port, int pmc_reset)
{
	volatile uint32_t v32;
	uint16_t p_idx, c_idx, timeout;

	if (priv->sys_port_umac[port])
	{
		v32 = priv->sys_port_umac[port]->SYSTEMPORT_UMAC_CMD;
		v32 &= ~SYSPORT_UNIMAC_COMMAND_CONFIG_RX_EN; /* disable RX */
		priv->sys_port_umac[port]->SYSTEMPORT_UMAC_CMD = v32;
	}
	if(priv->sys_port_gib != NULL)
	{
		v32 = priv->sys_port_gib->SYSTEMPORT_GIB_CONTROL;
		v32 &= ~SYSPORT_GIB_CONTROL_RX_EN;
		priv->sys_port_gib->SYSTEMPORT_GIB_CONTROL = v32;
	}
	// in case there is still DMA going on, wait for the last bit to finish	
	udelay (1000);

	// Disable and Flush RX DMA
	priv->sys_port_rdma[port]->SYSTEMPORT_RDMA_CONTROL &= ~SYSPORT_RDMA_CTRL_RDMA_EN_M;
	priv->sys_port_topctrl[port]->SYSTEMPORT_TOPCTRL_RX_FLUSH_CNTL=1;

	if(priv->sys_port_gib != NULL)
	{
		v32 = priv->sys_port_gib->SYSTEMPORT_GIB_CONTROL;
		v32 &= ~SYSPORT_GIB_CONTROL_RX_FLUSH;
		priv->sys_port_gib->SYSTEMPORT_GIB_CONTROL = v32;
	}

	// Disable TX DMA
	priv->sys_port_tdma[port]->SYSTEMPORT_TDMA_CONTROL &= ~SYSPORT_TDMA_CONTROL_TDMA_EN_M;
	// wait till TXDMA is completed
	timeout = 1000;
	v32 = priv->sys_port_tdma[port]->SYSTEMPORT_TDMA_DESC_RING_PRODUCER_CONSUMER_INDEX[0];

	p_idx = (uint16_t)(v32 & SYSTEMPORT_TDMA_DESC_RING_XX_PRODUCER_CONSUMER_INDEX_PRODUCER_INDEX_M);
	c_idx = (uint16_t)(v32 >> SYSTEMPORT_TDMA_DESC_RING_XX_PRODUCER_CONSUMER_INDEX_CONSUMER_INDEX_S);

	while ((p_idx != c_idx) && timeout)
	{
		udelay(1000);  	// will need to increase wait time if system port speed is slower 
						// (e.g, it takes 1.2ms to transmit 1518 bytes packets when system port is running at 10 Mbps)
		timeout --;
		v32 = priv->sys_port_tdma[port]->SYSTEMPORT_TDMA_DESC_RING_PRODUCER_CONSUMER_INDEX[0];
		p_idx = (uint16_t)(v32 & SYSTEMPORT_TDMA_DESC_RING_XX_PRODUCER_CONSUMER_INDEX_PRODUCER_INDEX_M);
		c_idx = (uint16_t)(v32 >> SYSTEMPORT_TDMA_DESC_RING_XX_PRODUCER_CONSUMER_INDEX_CONSUMER_INDEX_S);
	}
	printf("system port tx wait %d timeout %d p %d c %d\n", port, timeout, p_idx, c_idx);
	priv->sys_port_tdma[port]->SYSTEMPORT_TDMA_DESC_RING_PRODUCER_CONSUMER_INDEX[0] = 0;	

	if (priv->sys_port_umac[port])
	{
		v32 = priv->sys_port_umac[port]->SYSTEMPORT_UMAC_CMD;
		v32 &= ~SYSPORT_UNIMAC_COMMAND_CONFIG_TX_EN; /* disable RX */
		priv->sys_port_umac[port]->SYSTEMPORT_UMAC_CMD = v32;
	}
	if(priv->sys_port_gib != NULL)
	{
		v32 = priv->sys_port_gib->SYSTEMPORT_GIB_CONTROL;
		v32 &= ~SYSPORT_GIB_CONTROL_TX_EN;
		priv->sys_port_gib->SYSTEMPORT_GIB_CONTROL = v32;
	}
	priv->sys_port_topctrl[port]->SYSTEMPORT_TOPCTRL_TX_FLUSH_CNTL=1;
	udelay(100);

	priv->sys_port_topctrl[port]->SYSTEMPORT_TOPCTRL_RX_FLUSH_CNTL=0;
	priv->sys_port_topctrl[port]->SYSTEMPORT_TOPCTRL_TX_FLUSH_CNTL=0;

	if(priv->sys_port_gib != NULL)
	{
		v32 = priv->sys_port_gib->SYSTEMPORT_GIB_CONTROL;
		v32 &= ~SYSPORT_GIB_CONTROL_TX_FLUSH;
		v32 &= ~SYSPORT_GIB_CONTROL_RX_FLUSH;
		priv->sys_port_gib->SYSTEMPORT_GIB_CONTROL = v32;
	}

	if (pmc_reset)
	{
		pmc_sysport_reset_system_port(port);
	}
}

static int sysport_probe(struct udevice *dev)
{
	struct resource res;
	int ret=0, size, i;
	struct sysport_priv *priv = dev_get_priv(dev);
	struct udevice *ethsw_devp;
	bcm_ethsw_ops_t *ethsw_ops;
	uint phandle_id;
	const fdt32_t *list;

	priv->active_sysport=SYSPORT_0;

	priv->buffers[0]=memalign(ARCH_DMA_MINALIGN, NUM_DMA_BUFFERS*NET_DMA_BUFSIZE);
	if (priv->buffers[0] == NULL) {
		printf("%s: malloc failed\n", __func__);
		return -ENOMEM;
	}
	memset(priv->buffers[0], 0x0, NUM_DMA_BUFFERS*NET_DMA_BUFSIZE);
	flush_cache((unsigned long)(priv->buffers[0]), NUM_DMA_BUFFERS*NET_DMA_BUFSIZE);
	
	priv->buffers[1]=memalign(ARCH_DMA_MINALIGN, NUM_DMA_BUFFERS*NET_DMA_BUFSIZE);
	if (priv->buffers[1] == NULL) {
		printf("%s: malloc failed\n", __func__);
		return -ENOMEM;
	}
	memset(priv->buffers[1], 0x0, NUM_DMA_BUFFERS*NET_DMA_BUFSIZE);
	flush_cache((unsigned long)(priv->buffers[1]), NUM_DMA_BUFFERS*NET_DMA_BUFSIZE);
	
	ret = dev_read_resource_byname(dev, "systemport-rbuf-base", &res);
	if (ret) {
		dev_err(dev, "can't get regs systemport-rbuf--base address(ret = %d)!\n", ret);
		return ret;
	}
	priv->sys_port_rbuf[0]=devm_ioremap(dev, res.start, resource_size(&res));
	ret = dev_read_resource_byname(dev, "systemport-rdma-base", &res);
	if (ret) {
		dev_err(dev, "can't get regs systemport-rdma-base address(ret = %d)!\n", ret);
		return ret;
	}
	priv->sys_port_rdma[0]=devm_ioremap(dev, res.start, resource_size(&res));
	ret = dev_read_resource_byname(dev, "systemport-tdma-base", &res);
	if (ret) {
		dev_err(dev, "can't get regs systemport-tdma-base address(ret = %d)!\n", ret);
		return ret;
	}
	priv->sys_port_tdma[0]=devm_ioremap(dev, res.start, resource_size(&res));
	ret = dev_read_resource_byname(dev, "systemport-gib-base", &res);
	if (!ret) {
		//dev_err(dev, "can't get regs systemport-gib-base address(ret = %d)!\n", ret);
		priv->sys_port_gib=devm_ioremap(dev, res.start, resource_size(&res));
	}

	ret = dev_read_resource_byname(dev, "systemport-umac-base", &res);
	if (!ret) {
		//dev_err(dev, "can't get regs systemport-umac-base address(ret = %d)!\n", ret);
		//return ret;
		priv->sys_port_umac[0]=devm_ioremap(dev, res.start, resource_size(&res));
	}
	ret = dev_read_resource_byname(dev, "systemport-topctrl-base", &res);
	if (ret) {
		dev_err(dev, "can't get regs systemport-topctrl-base address(ret = %d)!\n", ret);
		return ret;
	}
	priv->sys_port_topctrl[0]=devm_ioremap(dev, res.start, resource_size(&res));

	// read in systemport 1 registers
	ret = dev_read_resource_byname(dev, "systemport1-rbuf-base", &res);
	if (!ret) {
		priv->sys_port_rbuf[1]=devm_ioremap(dev, res.start, resource_size(&res));
	}
	ret = dev_read_resource_byname(dev, "systemport1-rdma-base", &res);
	if (!ret) {
		priv->sys_port_rdma[1]=devm_ioremap(dev, res.start, resource_size(&res));
	}
	ret = dev_read_resource_byname(dev, "systemport1-tdma-base", &res);
	if (!ret) {
		priv->sys_port_tdma[1]=devm_ioremap(dev, res.start, resource_size(&res));
	}

	ret = dev_read_resource_byname(dev, "systemport1-umac-base", &res);
	if (!ret) {
		priv->sys_port_umac[1]=devm_ioremap(dev, res.start, resource_size(&res));
	}
	ret = dev_read_resource_byname(dev, "systemport1-topctrl-base", &res);
	if (!ret) {
		priv->sys_port_topctrl[1]=devm_ioremap(dev, res.start, resource_size(&res));
	}

	if (priv->sys_port_rbuf[1]) {
#if defined (CONFIG_BCMBCA_PMC_SYSPORT)
		priv->active_sysport=SYSPORT_1;
		pmc_sysport_power_up();
#endif
	}
	ret = dev_read_resource_byname(dev, "systemport-switchmdio-base", &res);
	priv->switch_mdio=NULL;
	if (!ret) {
		priv->switch_mdio=devm_ioremap(dev, res.start, resource_size(&res));
		phy_set_mdio_base(priv->switch_mdio);
	}

	ret = dev_read_resource_byname(dev, "phy-test-ctrl", &res);
	priv->phy_test_ctrl=NULL;
	if (!ret) {
		priv->phy_test_ctrl=devm_ioremap(dev, res.start, resource_size(&res));
	}

	ret = dev_read_resource_byname(dev, "sphy-ctrl", &res);
	priv->sphy_ctrl=NULL;
	if (!ret) {
		priv->sphy_ctrl=devm_ioremap(dev, res.start, resource_size(&res));

		// sysport driver only takes care of of PHY power on for 47622 when sphy is specified
		priv->phy_base = dev_read_u32_default(dev, "phy_base", 1);
		priv->phy_wkard_timeout = 0;
		dev_read_u32(dev, "phy_wkard_timeout", &priv->phy_wkard_timeout);

		printf("calling phy_wkard_timeout %d\n", priv->phy_wkard_timeout);
		gphy_powerup(priv->phy_base, priv->phy_wkard_timeout, priv->sphy_ctrl, NULL, priv->phy_test_ctrl);
		phy_advertise_caps(priv->phy_base | ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID);
	}
	// look for the switch driver
	for (i = 0; i < MAX_ETHSW; i++ )
	{
		priv->ethsw_devp[i] = NULL;
		list = dev_read_prop(dev, (i)? "ethsw_ext":"ethsw", &size);
		if (size > 0)
		{
			phandle_id = fdt32_to_cpu(*list);
			if (!uclass_get_device_by_phandle_id(UCLASS_NOP, phandle_id, &ethsw_devp))
			{
				printf("%s eth switch found, should have been probed name %s driver name %s\n", i? "ext":"", 
				ethsw_devp->name, ethsw_devp->driver->name);
				ethsw_ops = dev_get_priv(ethsw_devp);
				priv->ethsw_devp[i] = ethsw_devp;
				priv->ethsw_ops[i] = ethsw_ops;
				ethsw_ops->init (ethsw_devp);
			}
		}
	}

	return 0;
}

static const struct udevice_id sysport_bcmbca_match_ids[] = {
	{ .compatible = "brcm,bcmbca-systemport-v2.0"},
	{ }
};


U_BOOT_DRIVER(sysport) = {
	.name	= "sysport",
	.id	= UCLASS_ETH,
	.of_match = sysport_bcmbca_match_ids,
	.flags = DM_REMOVE_ACTIVE_ALL,
//	.ofdata_to_platdata = sysport_ofdata_to_platdata,
	.probe	= sysport_probe,
	.remove	= sysport_remove,
	.ops	= &sysport_ops,
	.priv_auto_alloc_size = sizeof(struct sysport_priv),
	.platdata_auto_alloc_size = sizeof(struct eth_pdata),
};
