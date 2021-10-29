/*****************************************************************************
 * Include file : IGMP.h
 *****************************************************************************
 * Contains the differents structures and constants for the IGMP-router 
 * Auteur: Lahmadi.Abdelkader@loria.fr 
 * MAJ: 7 Aout 2001
 ****************************************************************************/

#include <linux/igmp.h>

#define	IGMP_FMODE_INCLUDE	1
#define	IGMP_FMODE_EXCLUDE	0

#define IGMP_MAX_HOST_REPORT_DELAY	10 /* max delay for response to query (in seconds) */
#define IGMP_TIMER_SCALE	10
#define IGMP_DEF_QI	        125
#define IGMP_DEF_QRI		10

#define IGMP_DEF_RV	       	2
#define IGMP_OQPI		((IGMP_DEF_RV * IGMP_DEF_QI) + IGMP_DEF_QRI/2)
#define IGMP_GMI		((IGMP_DEF_RV * IGMP_DEF_QI) + IGMP_DEF_QRI)
#define IGMP_WAN_VERSION_TIMER	150		//IGMP_DEF_QI * 2 /* Foxconn add by aspen Bai, 01/08/2008 */ 

#define IGMP_MEMBERSHIP_QUERY		0x11
#define IGMP_V1_MEMBERSHIP_REPORT   0x12
#define IGMP_V2_MEMBERSHIP_REPORT   0x16
#define IGMP_V2_LEAVE_GROUP			0x17    /* Foxconn add by aspen Bai, 12/18/2007 */
#define IGMP_V3_MEMBERSHIP_REPORT   0x22

/* Foxconn add start by aspen Bai, 01/07/2008 */
#define MODE_IS_INCLUDE		1
#define MODE_IS_EXCLUDE		2
#define CHANGE_TO_INCLUDE	3
#define CHANGE_TO_EXCLUDE	4
#define ALLOW_NEW_SOURCES	5
#define BLOCK_OLD_SOURCES	6
/* Foxconn add end by aspen Bai, 01/07/2008 */

/* Foxconn add start by aspen Bai, 01/07/2008 */
#define GROUP_INIT    0
#define FIRST_JOIN_GROUP    1
#define LAST_LEAVE_GROUP	2
#define FIRST_JOIN_GROUP_HAD_SEND 3
#define LAST_LEAVE_GROUP_HAD_SEND 4
/* Foxconn add end by aspen Bai, 01/07/2008 */

#define IGMP_VERSION_1		0x12
#define IGMP_VERSION_2		0x16
#define IGMP_VERSION_3		0x22

#define	IGMP_MINLEN	       	8

#define	IGMP_ALL_ROUTERS	"224.0.0.2"
#define	IGMP_ALL_ROUTERS_V3	"224.0.0.22"


#define IGMP_SRSP(x)		((x)->igmpq_misc & (0x08))

#ifndef INADDR_ALLRTRS_IGMPV3_GROUP
#define INADDR_ALLRTRS_IGMPV3_GROUP 0xe0000016U
#endif

/* IGMP source filter type */
typedef struct _igmp_src_t {
  struct in_addr          igmps_source;
  int                     igmps_timer;
  /* add a flag to indicate the forwarding state*/	
  int                     igmps_fstate;
  struct _igmp_src_t      *igmps_next;
} igmp_src_t;

/* IGMP member type */
typedef struct _igmp_rep_t {
  struct in_addr igmpr_addr;
  struct _igmp_rep_t *igmpr_next;
}igmp_rep_t;

/* IGMP group type */
typedef struct _igmp_group_t {
  struct in_addr          igmpg_addr;
  int                     igmpg_timer;
  int                     igmpg_fmode;
  int					  igmpg_flags; /* Foxconn add by aspen Bai, 01/07/2008 */
  int					  igmpg_type;  /* Foxconn add by aspen Bai, 01/07/2008 */
  int 					  igmpg_version;
  igmp_rep_t*             igmpg_members;
  igmp_src_t*      	    igmpg_sources;
  struct _igmp_group_t*   igmpg_next;
} igmp_group_t;

/* Scheduled group specifiq query type*/
typedef struct _sch_query_t {
  struct in_addr gp_addr;
  int    igmp_retnum;
  int    numsrc;
  struct in_addr sources[1];
  struct _sch_query_t *sch_next;

} sch_query_t; 
/* IGMP query format */
typedef struct _igmpv3q_t {
  u_char          igmpq_type; 	/* version & type of IGMP message */
  u_char          igmpq_code; 	/* subtype for routing msgs */
  u_short         igmpq_cksum;	/* IP-style checksum */
  struct in_addr  igmpq_group;	/* group address being reported */
  u_char          igmpq_misc;  	/* reserved, supress and robustness */
  u_char          igmpq_qqi;   	/* querier's query interval */
  u_short         igmpq_numsrc;   /* number of sources */
  struct in_addr  igmpq_sources[1]; /* source addresses */
} igmpv3q_t;

/* IGMPv1/IGMPv2 report format */
typedef struct _igmpr_t {
  u_char          igmpr_type;  /* version & type */
  u_char          igmpr_code;  /* unused */
  u_short         igmpr_cksum; /* IP-style checksum */
  struct in_addr  igmpr_group; /* group address being reported */
} igmpr_t;

/* IGMPv3 group record format */
typedef struct _igmp_grouprec_t {
  u_char          igmpg_type;			/* record type */
  u_char          igmpg_datalen;		/* amount of aux data */
  u_short         igmpg_numsrc;		/* number of sources */
  struct in_addr  igmpg_group;		/* the group being reported */
  /* struct in_addr  igmpg_sources[1];	source addresses */
} igmp_grouprec_t;

/* IGMPv3 report format */
typedef struct _igmp_report_t {
  u_char          igmpr_type;		/* version & type of IGMP message */
  u_char          igmpr_code;		/* subtype for routing msgs */
  u_short         igmpr_cksum; 	/* IP-style checksum */
  u_short         igmpr_rsv;		/* reserved */
  u_short         igmpr_numgrps;      /* number of groups*/
  igmp_grouprec_t igmpr_group[1]; /* group records */
} igmp_report_t;

/* multicast source set */
typedef struct _igmp_mulsrc_t
{
	struct in_addr	        igmps_addr;
	struct igmp_mulsrc_t*   igmps_next;	
} igmp_mulsrc_t;
