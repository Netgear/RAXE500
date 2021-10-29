/* leases.h */
#ifndef _LEASES_H
#define _LEASES_H

#define MAX_HOSTNAME_LEN 63
#ifdef NETDUMA_EXTENSIONS
#define HOSTNAME_SHOWFILE       "/tmp/dhcpd_hostlist"
#endif /* NETDUMA_EXTENSIONS */

struct dhcpOfferedAddr {
	u_int8_t chaddr[16];
	u_int32_t yiaddr;	/* network order */
	u_int32_t expires;	/* host order */
	char hostname[MAX_HOSTNAME_LEN + 1];
};

extern unsigned char blank_chaddr[];

void clear_lease(u_int8_t *chaddr, u_int32_t yiaddr);
struct dhcpOfferedAddr *add_lease(u_int8_t *chaddr, u_int32_t yiaddr, unsigned long lease);
int lease_expired(struct dhcpOfferedAddr *lease);
struct dhcpOfferedAddr *oldest_expired_lease(void);
struct dhcpOfferedAddr *find_lease_by_chaddr(u_int8_t *chaddr);
struct dhcpOfferedAddr *find_lease_by_yiaddr(u_int32_t yiaddr);
u_int32_t find_address(int check_expired, unsigned char *chaddr);
u_int32_t find_address2(int check_expired, unsigned char *chaddr);
int check_ip(u_int32_t addr);
int check_arp_ip(u_int32_t addr, unsigned char *chaddr);

#ifdef NETDUMA_EXTENSIONS
extern void show_clients_hostname(void);
#endif /* NETDUMA_EXTENSIONS */

#endif
