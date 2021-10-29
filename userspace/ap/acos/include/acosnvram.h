/***************************************************************************
#***
#***    Copyright 2005  Hon Hai Precision Ind. Co. Ltd.
#***    All Rights Reserved.
#***    No portions of this material shall be reproduced in any form without the
#***    written permission of Hon Hai Precision Ind. Co. Ltd.
#***
#***    All information contained in this document is Hon Hai Precision Ind.  
#***    Co. Ltd. company private, proprietary, and trade secret property and 
#***    are protected by international intellectual property laws and treaties.
#***
#****************************************************************************/

#ifndef _acosnvram_h
#define _acosnvram_h

#include <acosTypes.h>

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#endif


#define PATH_DEV_NVRAM "/etc/parameter"
#define NVRAM_CFG_FILENAME  "/fl/NVRAM"
/* wklin modified start */
#define NVRAM_MAX_PARAM_LEN (4*1024) /* max len for a sigle parameter */
/* wklin modified end */
struct nvram_header {
	unsigned long magic;
	unsigned long len;
        unsigned short unused;
	unsigned short checksum;
	unsigned long version;
	unsigned long reserved[4];
};

struct nvram_tuple {
	char *name;
	char *value;
	struct nvram_tuple *next;
};

/* Compatibility */
typedef struct nvram_tuple EnvRec;

#ifdef ACOS_NVRAM
/****************************************************************
 ***    To avoid nvram lib conflic, make acos_nvram APIs
 ***    to be a warpper if use other nvram solution or else
 ***    use our own nvram implementation
 ****************************************************************/
extern int acos_nvram_loaddefault(void);
extern int acos_nvram_init(void);
extern void acos_nvram_exit(void);
extern char *acos_nvram_get(const char *name);

static INLINE char* acos_nvram_safe_get(const char *name, char *buf, int len)
{
    return acos_nvram_get(name) ? : "";
}

static INLINE int acos_nvram_match(char *name, char *match) {
    const char *value = acos_nvram_get(name);
    return (value && !strcmp(value, match));
}

static INLINE int
acos_nvram_invmatch(char *name, char *invmatch) {
    const char *value = acos_nvram_get(name);
    return (value && strcmp(value, invmatch));
}

extern int acos_nvram_set(const char *name, const char *value);
extern int acos_nvram_unset(const char *name);
extern int acos_nvram_commit(void);
extern int acos_nvram_getall(char *buf, int count);
extern int acos_nvram_dump(char *filter);
extern int acos_nvram_readflash(char *buf);
extern int acos_nvram_writeflash(char *buf, int size);
#else
extern int nvram_loaddefault(void);

/*
 * Initialize NVRAM access. May be unnecessary or undefined on certain
 * platforms.
 */
extern int nvram_init(void);

/*
 * Disable NVRAM access. May be unnecessary or undefined on certain
 * platforms.
 */
extern void nvram_exit(void);

/*
 * Get the value of an NVRAM variable. The pointer returned may be
 * invalid after a set.
 * @param	name	name of variable to get
 * @return	value of variable or NULL if undefined
 */
/* wklin modified start */
#if defined(_XDSL_PRODUCT)
extern char * _nvram_get(const char *name, char *buf, int buf_len);
extern char * nvram_get(const char *name);
#else
//extern char * nvram_get(const char *name, char *buf, int buf_len);
extern char *nvram_get(const char *name); /* Foxconn modified by Max Ding, 08/08/2019 */
#endif
/* 
 * Get the value of an NVRAM variable.
 * @param	name	name of variable to get
 * @return	value of variable or NUL if undefined
 */
#if defined(_XDSL_PRODUCT)
#define nvram_safe_get(name, buf, len) (_nvram_get(name, buf, len) ? : "")
#else
//#define nvram_safe_get(name, buf, len) (nvram_get(name, buf, len) ? : "")
#define nvram_safe_get(name, buf, len) (nvram_get(name) ? : "") /* Foxconn modified by Max Ding, 08/08/2019 */
#endif
/*
 * Match an NVRAM variable.
 * @param	name	name of variable to match
 * @param	match	value to compare against value of variable
 * @return	TRUE if variable is defined and its value is string equal
 *		to match or FALSE otherwise
 */
static INLINE int
nvram_match(char *name, char *match) {
#if defined(_XDSL_PRODUCT)
    char tmp[NVRAM_MAX_PARAM_LEN];
	const char *value = _nvram_get(name,tmp,NVRAM_MAX_PARAM_LEN);
#else
	//const char *value = nvram_get(name,tmp,NVRAM_MAX_PARAM_LEN);
	const char *value = nvram_get(name); /* Foxconn modified by Max Ding, 08/08/2019 */
#endif
	return (value && !strcmp(value, match));
}

/*
 * Inversely match an NVRAM variable.
 * @param	name	name of variable to match
 * @param	match	value to compare against value of variable
 * @return	TRUE if variable is defined and its value is not string
 *		equal to invmatch or FALSE otherwise
 */
static INLINE int
nvram_invmatch(char *name, char *invmatch) {
#if defined(_XDSL_PRODUCT)
    char tmp[NVRAM_MAX_PARAM_LEN];
	const char *value = _nvram_get(name, tmp, NVRAM_MAX_PARAM_LEN);
#else
	//const char *value = nvram_get(name, tmp, NVRAM_MAX_PARAM_LEN);
	const char *value = nvram_get(name); /* Foxconn modified by Max Ding, 08/08/2019 */
#endif
	return (value && strcmp(value, invmatch));
}

/*
 * Set the value of an NVRAM variable. The name and value strings are
 * copied into private storage. Pointers to previously set values
 * may become invalid. The new value may be immediately
 * retrieved but will not be permanently stored until a commit.
 * @param	name	name of variable to set
 * @param	value	value of variable
 * @return	0 on success and errno on failure
 */
extern int nvram_set(const char *name, const char *value);

/*
 * Unset an NVRAM variable. Pointers to previously set values
 * remain valid until a set.
 * @param	name	name of variable to unset
 * @return	0 on success and errno on failure
 * NOTE: use nvram_commit to commit this change to flash.
 */
extern int nvram_unset(const char *name);

/*
 * Commit NVRAM variables to permanent storage. All pointers to values
 * may be invalid after a commit.
 * NVRAM values are undefined after a commit.
 * @return	0 on success and errno on failure
 */
extern int nvram_commit(void);

/*
 * Get all NVRAM variables (format name=value\0 ... \0\0).
 * @param	buf	buffer to store variables
 * @param	count	size of buffer in bytes
 * @return	0 on success and errno on failure
 */
extern int nvram_getall(char *buf, int count);

/* used for config backup and restore */
extern int nvram_readflash(char *buf);
extern int nvram_writeflash(char *buf, int size);
#endif

#define NVRAM_MAGIC		0x40112791
#define NVRAM_VERSION		1
#define NVRAM_HEADER_SIZE	32
#if defined(_XDSL_PRODUCT) || (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 13))
#if (defined FLASH_SECTOR_128K)
#define NVRAM_SPACE		0x20000
#else
#define NVRAM_SPACE		0x10000
#endif
#else
#if defined(NVRAM_SIZE_16K)
#define NVRAM_SPACE		0x4000
#elif defined(NVRAM_SIZE_128K)
#define NVRAM_SPACE		0x20000
#else
#define NVRAM_SPACE		0x10000
#endif
#endif

extern unsigned short hd_cksum(unsigned short *addr, int len);

#endif /* _acosnvram_h */
