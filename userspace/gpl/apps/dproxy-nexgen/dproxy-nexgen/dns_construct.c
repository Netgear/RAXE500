#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dns_construct.h"
#include "dns_dyn_cache.h"
#include <time.h>

#define SET_UINT16_TO_N(buf, val, count) *(uint16*)buf = htons(val);count += 2; buf += 2
#define SET_UINT32_TO_N(buf, val, count) *(uint32*)buf = htonl(val);count += 4; buf += 4

#ifdef DNS_DYN_CACHE_MULTI_ANSWER
/*****************************************************************************/
static void dns_construct_reply_single_answer( dns_request_t *m , dns_dyn_list_t *dns_dyn_ent)
{
  debug("entering!");

  if( m->message.question[0].type == A ){
    /* standard lookup so return and IP */
    struct in_addr in;
    char ip[INET6_ADDRSTRLEN];
    int ttl;

    strcpy(ip, inet_ntoa(dns_dyn_ent->addr));
    ttl = labs(dns_dyn_ent->expiry - time(NULL));

    debug("ip = %s, ttl = %d", ip, ttl);

    inet_pton( AF_INET, ip, &in );
    SET_UINT16_TO_N( m->here, 0xc00c, m->numread ); /* pointer to name */
    SET_UINT16_TO_N( m->here, A, m->numread );      /* type */
    SET_UINT16_TO_N( m->here, IN, m->numread );     /* class */
    if(ttl > 0) {
      SET_UINT32_TO_N( m->here, ttl, m->numread );  /* ttl */
    }
    else {
      SET_UINT32_TO_N( m->here, 10000, m->numread );  /* ttl */
    }
    SET_UINT16_TO_N( m->here, 4, m->numread );      /* datalen */
    memcpy( m->here, &in.s_addr, sizeof(in.s_addr) ); /* data */
    m->here += 4;
    m->numread += sizeof( in.s_addr);
  }
}

/*****************************************************************************/
static int dns_construct_reply_answers( dns_request_t *m )
{
  dns_dyn_list_t *ent = NULL;
  int found = 0;
  int answer_count = 0;

  debug("m->cname = %s", m->cname);

  /* Search order: hosts, cache */
  debug("start searching dns_dyn_hosts!!");

  for(ent = dns_dyn_hosts; ent; ent = ent->next) {
    if (m->cname && strcasecmp(ent->cname, m->cname))
      continue;

    dns_construct_reply_single_answer(m, ent);    
    found = 1;    
    if(++answer_count >= NUM_RRS)
      break;
  }
  debug("(host entry)found = %d", found);

  if(found)
    return answer_count;

  debug("start searching dns_dyn_cache!!");

  for(ent = dns_dyn_cache; ent; ent = ent->next) {
    if (m->cname && strcasecmp(ent->cname, m->cname))
      continue;
    if (ent->expiry > time(NULL))
    {
      dns_construct_reply_single_answer(m, ent);
      if(++answer_count >= NUM_RRS)
        break;
    }
    else
    {
      dns_dyn_list_remove(&dns_dyn_cache, ent);
      ent = dns_dyn_cache;
    }
  }
  return answer_count;
}
#endif

/*****************************************************************************/
/* this function encode the plain string in name to the domain name encoding 
 * see decode_domain_name for more details on what this function does. */
int dns_construct_name(char *name, char *encoded_name)
{
  int i,j,k,n;

  k = 0; /* k is the index to temp */
  i = 0; /* i is the index to name */
  while( name[i] ){

	 /* find the dist to the next '.' or the end of the string and add it*/
	 for( j = 0; name[i+j] && name[i+j] != '.'; j++);
	 encoded_name[k++] = j;

	 /* now copy the text till the next dot */
	 for( n = 0; n < j; n++)
		encoded_name[k++] = name[i+n];
	
	 /* now move to the next dot */ 
	 i += j + 1;

	 /* check to see if last dot was not the end of the string */
	 if(!name[i-1])break;
  }
  encoded_name[k++] = 0;
  return k;
}
/*****************************************************************************/
int dns_construct_header(dns_request_t *m)
{
  char *ptr = m->original_buf;
  int dummy = 0;

  m->message.header.flags.flags = 0x8000; //response
  SET_UINT16_TO_N( ptr, m->message.header.id, dummy );
  SET_UINT16_TO_N( ptr, m->message.header.flags.flags, dummy );
  SET_UINT16_TO_N( ptr, m->message.header.qdcount, dummy );
  SET_UINT16_TO_N( ptr, m->message.header.ancount, dummy );
  SET_UINT16_TO_N( ptr, m->message.header.nscount, dummy );
  SET_UINT16_TO_N( ptr, m->message.header.arcount, dummy );
  
  return 0;
}
/*****************************************************************************/
void dns_construct_reply( dns_request_t *m )
{
  int len;

  /* point to end of orginal packet */ 
  m->here = &m->original_buf[m->numread];

  m->message.header.ancount = 1;
  m->message.header.flags.f.question = 1;
  dns_construct_header( m );

  if( m->message.question[0].type == A ){
#ifdef DNS_DYN_CACHE_MULTI_ANSWER
    int answer_count;

    answer_count = dns_construct_reply_answers(m);
    debug("answer_count = %d", answer_count);

    if(answer_count > 1)
    {
      char *ptr = m->original_buf;

      ptr += 6; // go to Answer RRs
      *(uint16*)ptr = htons(answer_count);
    }
#else
    /* standard lookup so return and IP */
      struct in_addr in;  
      inet_pton( AF_INET, m->ip, &in );
      SET_UINT16_TO_N( m->here, 0xc00c, m->numread ); /* pointer to name */
      SET_UINT16_TO_N( m->here, A, m->numread );      /* type */
      SET_UINT16_TO_N( m->here, IN, m->numread );     /* class */
    if(m->ttl > 0) {
      SET_UINT32_TO_N( m->here, m->ttl, m->numread );  /* ttl */
    }
    else {
      SET_UINT32_TO_N( m->here, 10000, m->numread );  /* ttl */
    }
      SET_UINT16_TO_N( m->here, 4, m->numread );      /* datalen */
      memcpy( m->here, &in.s_addr, sizeof(in.s_addr) ); /* data */
      m->numread += sizeof( in.s_addr);
#endif
  }else if ( m->message.question[0].type == PTR ){
      /* reverse look up so we are returning a name */
      SET_UINT16_TO_N( m->here, 0xc00c, m->numread ); /* pointer to name */
      SET_UINT16_TO_N( m->here, PTR, m->numread );    /* type */
      SET_UINT16_TO_N( m->here, IN, m->numread );     /* class */
    if(m->ttl > 0) {
      SET_UINT32_TO_N( m->here, m->ttl, m->numread );  /* ttl */
    }
    else {
      SET_UINT32_TO_N( m->here, 10000, m->numread );  /* ttl */
    }
      len = dns_construct_name( m->cname, m->here + 2 );
      SET_UINT16_TO_N( m->here, len, m->numread );      /* datalen */
      m->numread += len;
  }
}
/*****************************************************************************/
void dns_construct_error_reply(dns_request_t *m)
{
     /* point to end of orginal packet */ 
     m->here = m->original_buf;

     m->message.header.flags.f.question = 1;
     m->message.header.flags.f.rcode = 2;
     dns_construct_header( m );
}
