/* 
 * leases.c -- tools to manage DHCP leases 
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 */

#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "debug.h"
#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "leases.h"
#include "arpping.h"

unsigned char blank_chaddr[] = {[0 ... 15] = 0};

/* clear every lease out that chaddr OR yiaddr matches and is nonzero */
void clear_lease(u_int8_t *chaddr, u_int32_t yiaddr)
{
	unsigned int i, j;
	
	for (j = 0; j < 16 && !chaddr[j]; j++);
	
	for (i = 0; i < server_config.max_leases; i++)
		if ((j != 16 && !memcmp(leases[i].chaddr, chaddr, 16)) ||
		    (yiaddr && leases[i].yiaddr == yiaddr)) {
			memset(&(leases[i]), 0, sizeof(struct dhcpOfferedAddr));
		}
}


/* add a lease into the table, clearing out any old ones */
struct dhcpOfferedAddr *add_lease(u_int8_t *chaddr, u_int32_t yiaddr, unsigned long lease)
{
	struct dhcpOfferedAddr *oldest;
	
	/* clean out any old ones */
	clear_lease(chaddr, yiaddr);
		
	oldest = oldest_expired_lease();
	
	if (oldest) {
		memcpy(oldest->chaddr, chaddr, 16);
		oldest->yiaddr = yiaddr;
		oldest->expires = time(0) + lease;
	}
	
	return oldest;
}


/* true if a lease has expired */
int lease_expired(struct dhcpOfferedAddr *lease)
{
	return (lease->expires < (unsigned long) time(0));
}	


/* Find the oldest expired lease, NULL if there are no expired leases */
struct dhcpOfferedAddr *oldest_expired_lease(void)
{
	struct dhcpOfferedAddr *oldest = NULL;
	unsigned long oldest_lease = time(0);
	unsigned int i;

	
	for (i = 0; i < server_config.max_leases; i++)
		if (oldest_lease > leases[i].expires) {
			oldest_lease = leases[i].expires;
			oldest = &(leases[i]);
		}
	return oldest;
		
}


/* Find the first lease that matches chaddr, NULL if no match */
struct dhcpOfferedAddr *find_lease_by_chaddr(u_int8_t *chaddr)
{
	unsigned int i;

	for (i = 0; i < server_config.max_leases; i++)
		if (!memcmp(leases[i].chaddr, chaddr, 16)) return &(leases[i]);
	
	return NULL;
}


/* Find the first lease that matches yiaddr, NULL is no match */
struct dhcpOfferedAddr *find_lease_by_yiaddr(u_int32_t yiaddr)
{
	unsigned int i;

	for (i = 0; i < server_config.max_leases; i++)
		if (leases[i].yiaddr == yiaddr) return &(leases[i]);
	
	return NULL;
}

/*find and assignable address and check if the client hw address is a reserved address.*/
u_int32_t find_address2(int check_expired, unsigned char *chaddr)
{
	u_int32_t addr, ret;
	struct dhcpOfferedAddr *lease = NULL;		

    /* First check for reserved IP for this mac */
    addr = find_reserved_ip(chaddr);
    if (addr) {
        ret = htonl(addr);

        /* Make sure this IP is not used in network */
        if (!check_ip(addr) && !check_arp_ip(addr,chaddr)) {
            LOG(LOG_INFO, "find_address2: found reserved ip 0x%x.\n", ret);
            ret = htonl(addr);
            return ret;
        }
    }

	addr = ntohl(server_config.start); /* addr is in host order here */
    
	for (;addr <= ntohl(server_config.end); addr++) {

        LOG(LOG_INFO, "find_address2: assign ip 0x%x.\n", addr);
            /* ie, 192.168.55.0 */
        if (!(addr & 0xFF)) continue;
        
        /* ie, 192.168.55.255 */
        if ((addr & 0xFF) == 0xFF) continue;
            
            /*to check if this is a reserved ip for this mac or this ip has been reserved for others*/
            //if ( !check_reserved_ip(addr, chaddr) )  //foxconn modified, wenchia, 2007/09/10
            if ( !check_reserved_ip(htonl(addr), chaddr) ) 
            {
                LOG(LOG_INFO, "find_address2: ip 0x%x has beed reserved.\n", addr);
                continue;
            }
        
        /* lease is not taken */
        ret = htonl(addr);
        if ((!(lease = find_lease_by_yiaddr(ret)) ||
        
                /* or it expired and we are checking for expired leases */
                (check_expired  && lease_expired(lease))) &&
        
                /* and it isn't on the network */
                    !check_ip(ret) && !check_arp_ip(ntohl(addr),chaddr)) {
            return ret;
            break;
        }
	}
	return 0;
}

/* find an assignable address, it check_expired is true, we check all the expired leases as well.
 * Maybe this should try expired leases by age... */
u_int32_t find_address(int check_expired, unsigned char *chaddr) 
{
	u_int32_t addr, ret;
	struct dhcpOfferedAddr *lease = NULL;		

	addr = ntohl(server_config.start); /* addr is in host order here */
	for (;addr <= ntohl(server_config.end); addr++) {

		/* ie, 192.168.55.0 */
		if (!(addr & 0xFF)) continue;

		/* ie, 192.168.55.255 */
		if ((addr & 0xFF) == 0xFF) continue;

		/* lease is not taken */
		ret = htonl(addr);
		if ((!(lease = find_lease_by_yiaddr(ret)) ||

		     /* or it expired and we are checking for expired leases */
		     (check_expired  && lease_expired(lease))) &&

		     /* and it isn't on the network */
	    	     !check_ip(ret) && !check_arp_ip(addr,chaddr)) {
			return ret;
			break;
		}
	}
	return 0;
}


/* check is an IP is taken, if it is, add it to the lease table */
int check_ip(u_int32_t addr)
{
	struct in_addr temp;
	
	if (arpping(addr, server_config.server, server_config.arp, server_config.interface) == 0) {
		temp.s_addr = addr;
	 	LOG(LOG_INFO, "%s belongs to someone, reserving it for %ld seconds", 
	 		inet_ntoa(temp), server_config.conflict_time);
        /* Any need to reserve this IP ??? */
		/* add_lease(blank_chaddr, addr, server_config.conflict_time); */
		return 1;
	} else
           return 0;

	

}

/* check if there is an another entry occupied the ip in arp table  */
/*  like check_ip, if other sta already occupis the ip than return 1 */
int check_arp_ip(u_int32_t  addr, unsigned char *ch_addr)
{
#if 1
    return 0;
#else
    FILE *fp;
    int result;
    char str_ip[32];
    char str_mac[64];
   struct in_addr temp;
	
    temp.s_addr = addr;

   strcpy(str_ip,inet_ntoa(temp));

    sprintf(str_mac,"%02x:%02x:%02x:%02x:%02x",ch_addr[0],ch_addr[1],ch_addr[2],ch_addr[3],ch_addr[4]);

    system("cat /proc/net/arp > /tmp/arp_ip");
    
    fp=fopen("/tmp/arp_ip","r");	
    if(fp)
    {
        char buffer[128]; 
        while(fgets(buffer,sizeof(buffer)-1,fp))
        {
            char buffer_ip[128];
            sscanf(buffer,"%s",buffer_ip);
	     if(!strcmp(buffer_ip,str_ip))
            {
            
                fclose(fp);
            /*  find the ip is already in arp table */
		  if(!strstr(buffer,str_mac))
                {
                    return 1;
                }
		  else
		  {
                    return 0;
		  }
            }
        }
        fclose(fp);
    }
else
printf("can't open arp result");
//    unlink("/tmp/arp_ip");
    return 0;
#endif
}

/*foxconn Han edited start 09/05/2015*/
#ifdef LINK_AGG_IP_MANIPULATION

#define HASH_PORT   0x2
static int aggHashOrder = -1;

#define HASH_2_NAS      0x1
#define HASH_2_PORT3    0x2 
#define HASH_2_PORT4    0x4
#define HASH_ALGO       0x8

/*define HASH return value*/
#define HASH_ERR    3
#define HASH_NAS    2

#if 0
/*hash functions move to agghash prevent GPL conflict*/
u_int32_t _bond_xmit_hash
    (u_int8_t smac, u_int8_t dmac, u_int16_t pType, u_int32_t sip, u_int32_t dip)
{
    u_int32_t hash;
    hash = smac ^ dmac ^ pType;
    hash ^= sip ^ dip;
    hash ^= (hash >> 16);
    hash ^= (hash >> 8);
    LOG(LOG_INFO, "%s: 0x%X 0x%X 0x%X 0x%X 0x%X => hash=0x%X\n", __func__, smac, dmac, pType, sip, dip, hash);
    return hash;
}

u_int32_t bond_xmit_hash
    (u_int8_t dmac, u_int32_t dip)
{
    u_int32_t   hash;
    u_int16_t   pType   = ETH_P_IP;
    u_int32_t   sip     = server_config.server;
    u_int8_t    smac    = server_config.arp[5];

    hash = _bond_xmit_hash(smac, dmac, pType, sip,dip);

    return (hash % HASH_PORT);
}
#endif

u_int32_t bond_xmit_hash
    (u_int8_t dmac, u_int32_t dip)
{
    u_int32_t   hash = 0;
    char cmd[256];
	struct in_addr in;
    FILE * fp = NULL;
    char buf[8];

    in.s_addr = dip;
    sprintf(cmd,"agghash %s 00:00:00:00:00:%hhx > /tmp/dhcphash",inet_ntoa(in),dmac);
    LOG(LOG_INFO, "%s: hash cmd=%s,\n",__func__, cmd);
    system(cmd);

    fp = fopen("/tmp/dhcphash", "r");
    if(fp == NULL)
        return HASH_ERR;

    if(fgets(buf,8,fp)!=NULL)
    {
        if(buf[0] == '1')
            hash = 1;
        else if(buf[0] == '0')
            hash = 0;
        else if(buf[0] == '2')
            hash = HASH_NAS;
        else 
            hash = HASH_ERR;
    }
    
    if(fp)
        fclose(fp);
    return hash;
}


/*find and assignable address and check if the client hw address is a reserved address.*/
u_int32_t find_address_by_hash(int check_expired, unsigned char *chaddr)
{
    u_int32_t hash;
	u_int32_t addr, ret;
	struct dhcpOfferedAddr *lease = NULL;		

    /* First check for reserved IP for this mac */
    addr = find_reserved_ip(chaddr);
    if (addr) 
    {
        ret = htonl(addr);

        /* Make sure this IP is not used in network */
        if (!check_ip(addr) && !check_arp_ip(addr,chaddr)) 
        {
            LOG(LOG_INFO, "%s: found reserved ip 0x%x.\n", __func__,ret);
            ret = htonl(addr);
            return ret;
        }
    }

	addr = ntohl(server_config.start); /* addr is in host order here */
    
	for (;addr <= ntohl(server_config.end); addr++) {

        LOG(LOG_INFO, "%s: assign ip 0x%x.\n",__func__, addr);
        /* ie, 192.168.55.0 */
        if (!(addr & 0xFF)) continue;
        
        /* ie, 192.168.55.255 */
        if ((addr & 0xFF) == 0xFF) continue;

        /*to check if this is a reserved ip for this mac or this ip has been reserved for others*/
        if ( !check_reserved_ip(htonl(addr), chaddr) ) 
        {
            LOG(LOG_INFO, "%s: ip 0x%x has beed reserved.\n", __func__,addr);
            continue;
        }
        
        /* lease is not taken */
        ret = htonl(addr);
        if ((!(lease = find_lease_by_yiaddr(ret)) ||
                /* or it expired and we are checking for expired leases */
                (check_expired  && lease_expired(lease))) &&
                /* and it isn't on the network */
                !check_ip(ret) && !check_arp_ip(ret,chaddr)
            ) 
        {
            hash = bond_xmit_hash(chaddr[5], ret);
            LOG(LOG_INFO, "%s: mac 0x%X ip 0x%x .hash %x %x\n", __func__, chaddr[5], addr, hash, aggHashOrder);
            /*foxconn Han edited start, 09/11/2015 hash == 2 means reserve to NAS*/
            if(hash == HASH_NAS)
            {
                LOG(LOG_INFO, "%s: ip 0x%x should prior for NAS.\n", __func__,addr);
                continue;
            }
            //return ret;
            /*foxconn Han edited end, 09/11/2015*/

            if(hash != HASH_ERR && hash == aggHashOrder)
                continue;
            else
                aggHashOrder = hash;

            return ret;
            break;
        }
	}
	return 0;
}

#endif /*LINK_AGG_IP_MANIPULATION*/
/*foxconn Han edited end 09/05/2015*/
