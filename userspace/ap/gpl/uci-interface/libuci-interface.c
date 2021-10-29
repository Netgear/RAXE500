/***************************************************************************
***
***     Copyright 2009 Hon Hai Precision Industry Co., Ltd.
***     All Rights Reserved
***     No portions of this material may be reproduced in any form
***     without the written permission of:
***
***                 Hon Hai Precision Industry Co., Ltd.
***
***     All information contained in this document is Ambit Microsystems
***     Corporation company private, proprietary, and trade secret.
***
****************************************************************************
***
***    FILE NAME :
***         libuci-interface.c
***
***    DESCRIPTION :
***     
***
***    HISTORY :
***        - 2009/09/22 14:17, Jack Tseng
***          File created
***
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

//#include "uci.h"
#include "uci-interface.h"

/* jack tseng 01/14/2010 add: used for locking the file "tlock"  */
#include <sys/file.h>
#include <ctype.h>
#include <fcntl.h>
/* end of jack tseng 01/14/2010 */
#include <glob.h>

#define MAX_KEY_LIST  4096

#define TRANSACTION_BEGIN "begin"
#define TRANSACTION_END "end"

#define TRANSACTION_MAX_PACKAGES  64

#define UCI_DEFAULT_DIR     "/etc/config"
#define UCI_CHANGE_TRACE_DIR  "/tmp/delta"
#define UCI_CHANGE_TRACE_FILE_DELTA "delta"
#define UCI_GENERAL_DIR     "/etc/config/general"

#define UCI_TABLE_INDEX   "index"

/*
 * Global definition
 */
struct uci_type_list {
  unsigned int idx;
  const char *name;
  struct uci_type_list *next;
};

/* jack tseng 2010/09/06 add */
#define TYPE_SECTION  1
#define TYPE_OPTION   2
struct uci_merge_list {
  char *name;
  char *value;
  int type;
  struct uci_merge_list *next;
};
/* end of jack tseng 09/06/2010 */

/*
 * Global variable
 */
static struct uci_context *ctx=NULL;
static struct uci_context *trace_ctx=NULL; /* record the changes of each package */
char  appname[]="uci-interface";
static const char *delimiter = " ";

static struct uci_type_list *type_list = NULL;
static char *typestr = NULL;
static const char *cur_section_ref = NULL;

 
const char  changeSection[] = "change";
const char  changeSectionKey[] = "change.key";
const char  changeSectionLastKey[] = "change.lastkey";

/* for transaction */
const char transactionPackage[] = "transaction";
const char transactionSectionType[] = "record";
const char transactionStateKey[] = "state";
const char transactionPackagesKey[] = "packages";
const char transactionIdKey[] = "id";
//const char transactionResultKey[] = "result";

/* for transaction_result */
const char transactionResultPackage[] = "transaction_result";
const char transactionResultPackageKey[] = "package";
const char transactionResultResultKey[] = "result";


int fdLock=-1; /* jack tseng 01/14/2010 add: used for locking the file "tlock"  */

extern void *uci_malloc(struct uci_context *ctx, size_t size);


/*
 ******************************************
 * F1 UCI internal functions 
 *******************************************
 */
 static void cli_perror(void)
{
#ifdef DEBUG 
  uci_perror(ctx, appname);
#endif
}

/* jack tseng 03/22/2010 add */
int extract_table_token(char* tableKey,char* packageName,char* sectionType, char* sectionInstance,char* optionName)
{
  char *package=NULL; 
  char *section=NULL; 
  char *option=NULL; 
  char sep[] = ".";
  //int sameSectionCount=0;
  //int newSectionCount;
  //int newSectionIndex;
  int sectionLen;
  char *sectionTypeBegin;
  char  *sectionTypeEnd;
  char *sectionAddrEnd;

  if ((tableKey == NULL) ||
        (packageName == NULL) ||
          (sectionType == NULL) ||  
          (sectionInstance == NULL) ||
          (optionName == NULL) )
          return UCI_ERR_INVAL;
  /* extract the package name, section name and option name from input variable "key" */
  package = strtok(tableKey,sep);
  if (package != NULL)
  {
    strcpy(packageName,package);
    section = strtok(NULL,sep);
  
    if (section != NULL)
    {
      option = strtok(NULL,sep);
      if (option != NULL)
        strcpy(optionName,option);
      else
        optionName[0] = '\0';
    }
  }
  //printf("\n\rpackage:%s,section name:%s,option:%s",package,section,option);

  if ( (section != NULL) && (section[0]='@' ) ) /* the multiple instance */
  {
    memset(sectionType, 0, MEDIUM_KEY_SIZE);
    memset(sectionInstance, 0, TINY_KEY_SIZE);
    sectionLen = strlen(section);
    sectionAddrEnd = section+sectionLen; 
  /****** get the section type *****/
    sectionTypeBegin = sectionTypeEnd = section+1; /* bypass '@' */
    while( (sectionTypeEnd < sectionAddrEnd) && (*sectionTypeEnd != '['))
      sectionTypeEnd++;
    if (*sectionTypeEnd != '[')
      return UCI_ERR_INVAL;
    if (sectionTypeEnd > sectionTypeBegin)
    {
      strncpy(sectionType, sectionTypeBegin, sectionTypeEnd-sectionTypeBegin);
    }
    //printf("\n\rextract section type:%s",sectionType);
  /******* get the section instance *****/
    sectionTypeEnd++; /* bypass '[' */
    sectionTypeBegin = sectionTypeEnd;
    while((sectionTypeEnd < sectionAddrEnd) && (*sectionTypeEnd != ']') )
      sectionTypeEnd++;
    if (*sectionTypeEnd != ']')
      return UCI_ERR_INVAL;
    if (sectionTypeEnd > sectionTypeBegin)
    {
      strncpy(sectionInstance, sectionTypeBegin, sectionTypeEnd-sectionTypeBegin);
    }
    //printf("\n\rextract section instance:%s",sectionInstance);
  }
  else
    return UCI_ERR_INVAL;
  return UCI_OK;
}

/* 
 Find the section entry that has the value of option "index" to be the same as the input variable 'instance'.
*/
int find_table_index(char* package, char* section, char* instance, int* pIndex)
{
  struct uci_element *e;
  struct uci_section *s;
  struct uci_ptr ptr;
  int sameSectionCount=0;
  int ret = UCI_OK;
  char fullKey[LARGE_KEY_SIZE];
  char indexValue[TINY_KEY_SIZE];
  int bFound = 0;


  if (uci_lookup_ptr(ctx, &ptr, package, true) != UCI_OK) 
  {
    cli_perror();
    return ctx->err; //UCI_ERR_NOTFOUND;
  }       

  if (ptr.p != NULL)
  {
  /* check th amount of the section type that has already been exist in the package file */
    uci_foreach_element( &(ptr.p->sections), e) 
    {
      //printf("\n\rsection name:%s",e->name);
      s = uci_to_section(e);
      //printf("\n\rsection type:%s",s->type);
      if (strcmp(s->type,section)==0) /* the same section type */
      {
        /* get the value of option "index" if it is the same as the instance of the input key */
        sprintf(fullKey,"%s.@%s[%d].%s",package,section,sameSectionCount,UCI_TABLE_INDEX);
        //printf("\n\rfullKey:%s",fullKey);
        ret = uciw_get_value(fullKey, indexValue, TINY_KEY_SIZE);
        if (ret == UCI_OK)
        {
          if (strcmp(indexValue,instance)==0)  /* the same value of index */
          {
            *pIndex = sameSectionCount;
            bFound = 1;
            break;
          }
        } 
        sameSectionCount++;
      }
    }
  }
  if (bFound)
    return UCI_OK;
  else
    return UCI_ERR_NOTFOUND;
}
/* end of jack tseng 03/22/2010 */

int create_change_section(const char* packageName)
{
  char   changedSection[MAX_KEY_SIZE];
  struct uci_ptr ptrChnageSection;
  int ret;

#if 1
  sprintf(changedSection, "%s_%s.%s=%s",packageName,UCI_CHANGE_TRACE_FILE_DELTA,changeSection, "type");
#else
  sprintf(changedSection, "%s.%s=%s",packageName,changeSection, "type");
#endif
  //printf("\n\r[%s,%d]changedSection:%s\n\r",__FUNCTION__,__LINE__,changedSection);
#if 1
  if (uci_lookup_ptr(trace_ctx, &ptrChnageSection, changedSection, true) != UCI_OK) 
  {
    cli_perror();
    return trace_ctx->err; //UCI_ERR_NOTFOUND;
  }
  ret = uci_set(trace_ctx, &ptrChnageSection);
/* jack tseng 04/08/2010 add */
  if (ret == UCI_OK)
  {
    ret = uci_commit(trace_ctx, &ptrChnageSection.p, false);
  }
/* end of jack tseng 04/08/2010 */
#else
  if (uci_lookup_ptr(ctx, &ptrChnageSection, changedSection, true) != UCI_OK) 
  {
    cli_perror();
    return ctx->err; //UCI_ERR_NOTFOUND;
  }
  ret = uci_set(ctx, &ptrChnageSection);
#endif  
  return ret;
}

int get_trace_value(char* key,char* value,int valueLen)
{
  struct uci_element *e;
  struct uci_ptr ptr;
  bool sep = false;
  //int ret = UCI_OK;
  
  if ((trace_ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

  //printf("\n\r[%s,%d]key:%s",__FUNCTION__,__LINE__,key);
  //fflush(stdout);
  if (uci_lookup_ptr(trace_ctx, &ptr, key, true) != UCI_OK) 
  {
    cli_perror();
    return trace_ctx->err; //UCI_ERR_NOTFOUND;
  }

  
  if (!(ptr.flags & UCI_LOOKUP_COMPLETE)) /* key not found */
  {
    trace_ctx->err = UCI_ERR_NOTFOUND;
    cli_perror();
    return UCI_ERR_NOTFOUND;
  }

  
  memset(value, 0, valueLen);
  e = ptr.last;
  switch(e->type) 
  {
  case UCI_TYPE_SECTION:
    //printf("%s\n\r", ptr.s->type);
    strncpy(value,ptr.s->type,(size_t)valueLen);
    value[valueLen-1]='\0';
    break;
  case UCI_TYPE_OPTION:
    switch(ptr.o->type) 
    {
    case UCI_TYPE_STRING:
      //printf("%s\n\r", o->v.string);
      //printf("\n\r[%s,%d]value:%s,valueLen:%d\n\r",__FUNCTION__,__LINE__,ptr.o->v.string,valueLen);
      if (strlen(ptr.o->v.string) >= valueLen)
        return UCI_ERR_MEM;
      strncpy(value,ptr.o->v.string,valueLen);
      value[valueLen-1]='\0';
      break;
    case UCI_TYPE_LIST:
      uci_foreach_element(&ptr.o->v.list, e) 
      {
        //printf("%s%s", (sep ? delimiter : ""), e->name);
        if (strlen(value) + strlen(e->name) + 1 >= valueLen)
          return UCI_ERR_MEM; 
        sprintf(value,"%s%s%s",value,(sep ? delimiter : ""), e->name);
        sep = true;
      }
      //printf("\n\r");
      break;
    default:
      printf("<unknown>\n\r");
      break;
    }
    break;
  default:
    break;
  }

  return UCI_OK;
}


void validate_trace_file(char* packageName, int reCreate)
{
  struct stat sb;
  char fullName[LARGE_KEY_SIZE];
  int fd;
  #if 0
  int ret=UCI_OK;
  #endif
  //struct uci_ptr ptrTrace;
  //char traceFile[64];

    
  /* check if the file is exist */
/* jack tseng 03/05/2010 modification: use the same directory with UCI_DEFAULT_DIR */
#if 1
  sprintf(fullName,"%s/%s_%s",trace_ctx->confdir,packageName,UCI_CHANGE_TRACE_FILE_DELTA);
  //sprintf(fullName,"%s/%s_%s",UCI_DEFAULT_DIR,packageName,UCI_CHANGE_TRACE_FILE_DELTA);
#else
  sprintf(fullName,"%s/%s",UCI_CHANGE_TRACE_DIR,packageName);
#endif
  if (reCreate)
  {
    remove(fullName);
  }
  
  if (stat(fullName, &sb) == -1)  /* file is not exist, create it */
    {
          fd = open(fullName, O_CREAT, S_IRWXU|S_IRWXG|S_IROTH | S_IWOTH);
    if (fd)
    {
      close(fd);
      #if 1
      create_change_section(packageName);
      #else
      ret = create_change_section(packageName);
      printf("\n\r[%s,%d]create_change_section ret:%d\n\r",__FUNCTION__,__LINE__,ret);
      #endif

    /* jack tseng 04/08/2010 remove */
    /* The operation if commit will be done in create_change_section() */
    #if 0
      /* commit change trace */
      #if 1
      sprintf(traceFile,"%s_%s",packageName,UCI_CHANGE_TRACE_FILE_DELTA);
      if (uci_lookup_ptr(trace_ctx, &ptrTrace, traceFile, true) != UCI_OK) 
      #else
      if (uci_lookup_ptr(trace_ctx, &ptrTrace, packageName, true) != UCI_OK) 
      #endif  
      {
        cli_perror();
        return trace_ctx->err; 
      }
      ret = uci_commit(trace_ctx, &ptrTrace.p, false);
    #endif
    }
  }
}


static void uci_show_value(struct uci_option *o)
{
  struct uci_element *e;
  bool sep = false;

  switch(o->type) 
  {
  case UCI_TYPE_STRING:
    printf("%s\n\r", o->v.string);
    break;
  case UCI_TYPE_LIST:
    uci_foreach_element(&o->v.list, e) 
    {
      printf("%s%s", (sep ? delimiter : ""), e->name);
      sep = true;
    }
    printf("\n\r");
    break;
  default:
    printf("<unknown>\n\r");
    break;
  }
}

static void uci_reset_typelist(void)
{
  struct uci_type_list *type;
  while (type_list != NULL) {
      type = type_list;
      type_list = type_list->next;
      free(type);
  }
  if (typestr) {
    free(typestr);
    typestr = NULL;
  }
  cur_section_ref = NULL;
}

static char *uci_lookup_section_ref(struct uci_section *s)
{
  struct uci_type_list *ti = type_list;
  int maxlen;

  //if (!s->anonymous || !(flags & CLI_FLAG_SHOW_EXT))
  if (!s->anonymous)  
    return s->e.name;

  /* look up in section type list */
  while (ti) {
    if (strcmp(ti->name, s->type) == 0)
      break;
    ti = ti->next;
  }
  if (!ti) {
    ti = malloc(sizeof(struct uci_type_list));
    memset(ti, 0, sizeof(struct uci_type_list));
    ti->next = type_list;
    type_list = ti;
    ti->name = s->type;
  }

  maxlen = strlen(s->type) + 1 + 2 + 10;
  if (!typestr) {
    typestr = malloc(maxlen);
  } else {
    typestr = realloc(typestr, maxlen);
  }
  sprintf(typestr, "@%s[%d]", ti->name, ti->idx);
  ti->idx++;
  return typestr;
}


static void uci_show_option(struct uci_option *o)
{
  printf("%s.%s.%s=",
    o->section->package->e.name,
    (cur_section_ref ? cur_section_ref : o->section->e.name),
    o->e.name);
  uci_show_value(o);
}

static void uci_show_section(struct uci_section *s)
{
  struct uci_element *e;
  const char *cname;
  const char *sname;

  cname = s->package->e.name;
  sname = (cur_section_ref ? cur_section_ref : s->e.name);
  printf("%s.%s=%s\n\r", cname, sname, s->type);
  uci_foreach_element(&s->options, e) {
    uci_show_option(uci_to_option(e));
  }
}

static void uci_show_package(struct uci_package *p)
{
  struct uci_element *e;

  uci_reset_typelist();
  uci_foreach_element( &p->sections, e) {
    struct uci_section *s = uci_to_section(e);
    cur_section_ref = uci_lookup_section_ref(s);
    uci_show_section(s);
  }
  uci_reset_typelist();
}

void uci_dump_package(char* package)
{
  struct uci_element *e;
  //struct uci_element *eShow;
  struct uci_ptr ptr;
  
  if (uci_lookup_ptr(ctx, &ptr, package, true) != UCI_OK) 
  {
    //printf("\n\r[uci_dump_package]uci_lookup_ptr error\n\r");
    //fflush(stdout);
    if (ctx->err > 0)
    {
      printf("\n\r[uci_dump_package]errno:%d\n\r",ctx->err);
      fflush(stdout);
    }
      
    cli_perror();
    return ;
  }

  e = ptr.last;
  if (e->type == UCI_TYPE_PACKAGE) 
  {
    uci_show_package(ptr.p);
  }
}

#ifdef OPENWRT 
int uciw_touch_package_file(char *confdir, char *path)
{
    char cmd[MEDIUM_KEY_SIZE];
    
    /*
     * https://wiki.openwrt.org/doc/techref/filesystems
     * Overlayfs's support for inotify mechanisms is not complete yet. 
     * Events like IN_CLOSE_WRITE cannot be notified to listening process.
     * Use 'touch' to trigger event when any config was commit.
     * modify 20160307
     * only for /etc/config/general now
     */
    if ( confdir == NULL || path == NULL) 
    {
        return UCI_ERR_INVAL;
    }

    if (0 != strncmp(confdir, UCI_GENERAL_DIR, strlen(UCI_GENERAL_DIR)))
    {
        return UCI_ERR_INVAL;
    }

    memset(cmd, 0, MEDIUM_KEY_SIZE);

    snprintf(cmd, MEDIUM_KEY_SIZE, "touch %s", path);

    system(cmd);

    return UCI_OK;
}
#endif

int uci_commit_package_do(char* package)
{
    struct uci_ptr ptr;
    struct uci_ptr ptrChange1,ptrChange2;
    //struct uci_package *p;
    //struct uci_element *e;
    char	 changedSection[MAX_MAX_KEY_SIZE];
    //char  changedKeyList[MAX_KEY_LIST];
    char value[MAX_MAX_KEY_SIZE],valueLastkey[MAX_MAX_KEY_SIZE];
    char* valueTruc;
    int ret;
    int index;
    struct uci_ptr ptrTrace;
    char traceFile[64];

    if ((ctx==NULL) || (package == NULL))
    {
        return UCI_ERR_INVAL;
    }

    //printf("\n\r[%s,%d]package:%s\n\r",__FUNCTION__,__LINE__,package);
    if (uci_lookup_ptr(ctx, &ptr, package, true) != UCI_OK) 
    {
        cli_perror();
        return ctx->err; 
    }

	memset(value,0,MAX_MAX_KEY_SIZE);
	memset(valueLastkey,0,MAX_MAX_KEY_SIZE);
    
/* save the modification of the package to a list for the use of other modules */
#if 1 
    //memset(changedKeyList, 0, MAX_KEY_LIST);  
    /* <1> get the list of the chnaged key from <package name>.change.key */
#if 1
    sprintf(changedSection, "%s_%s.%s",package,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey);
#else
    sprintf(changedSection, "%s.%s",package,changeSectionKey);
#endif

    //printf("\n\r[%s,%d]changedSection:%s\n\r",__FUNCTION__,__LINE__,changedSection);
    ret = get_trace_value(changedSection, value, MAX_MAX_KEY_SIZE);
    ret = UCI_OK;
    //printf("\n\r value:%s\n\r",value);

    /* if the the content of "<package>.change.key" is empty, 
     * it means that the <package> is not modified 
     */
    if (strlen(value) > 0)
    {
        /* <2> get the list of the previous chnaged key from <package name>.change.lastkey */
#if 1
        sprintf(changedSection, "%s_%s.%s",package,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionLastKey);
#else
        sprintf(changedSection, "%s.%s",package,changeSectionLastKey);
#endif
        //printf("\n\r[%s,%d]changedSection:%s\n\r",__FUNCTION__,__LINE__,changedSection);
		ret = get_trace_value(changedSection, valueLastkey, MAX_MAX_KEY_SIZE);
        //printf("\n\r valueLastkey:%s\n\r",valueLastkey);
  
        /* <3> save the list of changed key into <package name>.change.last-key */
        index=0;
        if (value[index]==' ') 
        {
          index++;
        }
        valueTruc = value+index; /* ignore the leading space */
    
#if 1
        sprintf(changedSection, "%s_%s.%s=%s",package,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionLastKey, valueTruc);
#else
        sprintf(changedSection, "%s.%s=%s",package,changeSectionLastKey, valueTruc);
#endif
        //printf("\n\r[%s,%d]changedSection:%s\n\r",__FUNCTION__,__LINE__,changedSection);
    
        if (uci_lookup_ptr(trace_ctx, &ptrChange1, changedSection, true) != UCI_OK) 
        {
            cli_perror();
            return trace_ctx->err; 
        }
        ret = uci_set(trace_ctx, &ptrChange1); /* ignore the return */

        /* <4> clear the content of <package name>.change.key */
#if 1
        sprintf(changedSection, "%s_%s.%s=%s",package,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, "");
#else
        sprintf(changedSection, "%s.%s=%s",package,changeSectionKey, "");
#endif
        if (uci_lookup_ptr(trace_ctx, &ptrChange2, changedSection, true) != UCI_OK) 
        {
            cli_perror();
            return trace_ctx->err; 
        }
        ret = uci_set(trace_ctx, &ptrChange2); /* ignore the return */
#endif  

        /* commit change trace */
#if 1
        sprintf(traceFile,"%s_%s",package,UCI_CHANGE_TRACE_FILE_DELTA);
        if (uci_lookup_ptr(trace_ctx, &ptrTrace, traceFile, true) != UCI_OK) 
#else
        if (uci_lookup_ptr(trace_ctx, &ptrTrace, package, true) != UCI_OK) 
#endif
        {
            cli_perror();
            return trace_ctx->err; 
        }
        ret = uci_commit(trace_ctx, &ptrTrace.p, false);
  
        //usleep(1000);
        //sleep(1);
  
        if (ret == UCI_OK)
        {
            /* commit */
            if (uci_commit(ctx, &ptr.p, false) != UCI_OK)
            {
                cli_perror();
                ret = ctx->err;
    
                /* restore the change key */  
#if 1
#if 1
                sprintf(changedSection, "%s_%s.%s=%s",package,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionLastKey, valueLastkey);
#else
                sprintf(changedSection, "%s.%s=%s",package,changeSectionLastKey, valueLastkey);
#endif
                //printf("\n\r[%s,%d]changedSection:%s\n\r",__FUNCTION__,__LINE__,changedSection);
                if (uci_lookup_ptr(trace_ctx, &ptrChange1, changedSection, true) != UCI_OK) 
                {
                    cli_perror();
                    return ctx->err;
                }
                uci_set(ctx, &ptrChange1);  /* ignore the return */

#if 1
                sprintf(changedSection, "%s_%s.%s=%s",package,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, value);
#else
                sprintf(changedSection, "%s.%s=%s",package,changeSectionKey, value);
#endif
                //printf("\n\r[%s,%d]changedSection:%s\n\r",__FUNCTION__,__LINE__,changedSection);
               if (uci_lookup_ptr(trace_ctx, &ptrChange1, changedSection, true) != UCI_OK) 
               {
                   cli_perror();
                   return ctx->err; 
               }
               uci_set(ctx, &ptrChange1); /* ignore the return */
#endif

               return ret;
            }

#ifdef OPENWRT 
            /*
             * https://wiki.openwrt.org/doc/techref/filesystems
             * Overlayfs's support for inotify mechanisms is not complete yet. 
             * Events like IN_CLOSE_WRITE cannot be notified to listening process.
             * Use 'touch' to trigger event when any config was commit.     
             * modify 20160307
             * only for /etc/config/general now
             */
            uciw_touch_package_file(ctx->confdir, ptr.p->path);
#endif

            uci_unload(ctx, ptr.p);
        }
        else
        {
            printf("\n\r commit the changes of %s failed",(char *)ptrTrace.p);
        }  
    }
    
    return ret;
}

/* jack tseng 11/17/2010 add for reset to default */
int uci_commit_package_do_force(char* package)
{
  struct uci_ptr ptr;
  int ret=UCI_OK;


  if ((ctx==NULL) || (package == NULL))
  {
    return UCI_ERR_INVAL;
  }


  //printf("\n\r[%s,%d]package:%s\n\r",__FUNCTION__,__LINE__,package);
  if (uci_lookup_ptr(ctx, &ptr, package, true) != UCI_OK) 
  {
    cli_perror();
    return ctx->err; 
  }


  /* commit */
  if (uci_commit(ctx, &ptr.p, false) != UCI_OK)
  {
    cli_perror();
    ret = ctx->err;
  
    return ret;
  }

  uci_unload(ctx, ptr.p);

  return ret;
}
/* end of jack tseng 11/16/2010 */

int create_new_key(struct uci_ptr *p_ptr,char* sType)
{
  struct stat sb;
  char fullName[LARGE_KEY_SIZE];
  char fullKey[LARGE_KEY_SIZE];
  int fd;
  int ret=UCI_OK;
  struct uci_element *e;
  struct uci_section *s;
  int sameSectionCount=0;
  int newSectionCount;
  int newSectionIndex;
  int sectionLen;
  char *sectionTypeBegin;
  char *sectionTypeEnd;
  char *sectionAddrEnd;
  char extractSectionType[MEDIUM_KEY_SIZE];
  char extractSectionInstance[TINY_KEY_SIZE];
  char * uci_ptr_value;
  char * uci_ptr_package;
  

  uci_ptr_value = (char *)p_ptr->value;
  uci_ptr_package = (char *)p_ptr->package;
    
/* check if the file is exist */
  sprintf(fullName,"%s/%s",ctx->confdir,p_ptr->package);
  if (stat(fullName, &sb) == -1)  /* file is not exist, create it */
  {
    fd = open(fullName, O_CREAT, S_IRWXU|S_IRWXG|S_IROTH | S_IWOTH);
    if (fd)
      close(fd);
  }
/* 
If the section type (sType) is NULL, the key will be "<package>.<section>" or 
will be multiple instance as "<package>.@<section type>[].<option>"
*/ 
  if (sType == NULL)
  {
    /* check if the key is the format of multiple instance */
    if (p_ptr->section[0] == '@') /* the format of multiple instanec */
    {
      //printf("\n[%s,%d]multiple instance",__FUNCTION__,__LINE__);
      memset(extractSectionType, 0, MEDIUM_KEY_SIZE);
      memset(extractSectionInstance, 0, TINY_KEY_SIZE);
      sectionLen = strlen(p_ptr->section);
      sectionAddrEnd = (char *)(p_ptr->section + sectionLen); 
    /****** get the section type *****/
      sectionTypeBegin = sectionTypeEnd = (char *)(p_ptr->section + 1); /* bypass '@' */
      while( (sectionTypeEnd < sectionAddrEnd) && (*sectionTypeEnd != '['))
        sectionTypeEnd++;
      if (*sectionTypeEnd != '[')
        return UCI_ERR_INVAL;
      if (sectionTypeEnd > sectionTypeBegin)
      {
        strncpy(extractSectionType, sectionTypeBegin, sectionTypeEnd-sectionTypeBegin);
      }
      //printf("\n\rextract section type:%s",extractSectionType);
    /******* get the section instance *****/
      sectionTypeEnd++; /* bypass '[' */
      sectionTypeBegin = sectionTypeEnd;
      while((sectionTypeEnd < sectionAddrEnd) && (*sectionTypeEnd != ']') )
        sectionTypeEnd++;
      if (*sectionTypeEnd != ']')
        return UCI_ERR_INVAL;
      if (sectionTypeEnd > sectionTypeBegin)
      {
        strncpy(extractSectionInstance, sectionTypeBegin, sectionTypeEnd-sectionTypeBegin);
      }
      //printf("\n\rextract section instance:%s",extractSectionInstance);

      if (p_ptr->p != NULL)
      {
      /* check th amount of the section type that has already been exist in the package file */
        uci_foreach_element( &(p_ptr->p->sections), e) 
        {
          //printf("\n\rsection name:%s",e->name);
          s = uci_to_section(e);
          printf("\n\rsection type:%s",s->type);
          if (strcmp(s->type,extractSectionType)==0)
            sameSectionCount++;
        }
      }
      //printf("\n\rsection instance count:%d",sameSectionCount);
      if (atoi(extractSectionInstance)==-1) /* handle index is -1  */
      {
        ret = uciw_add_section(uci_ptr_package, extractSectionType);
        if (ret == UCI_OK) /* create option */
        {
          sprintf(fullKey,"%s.%s.%s",p_ptr->package,p_ptr->section,p_ptr->option);
          //printf("\n[%s,%d]key:%s,value:%s",__FUNCTION__,__LINE__,fullKey,uci_ptr_value);
          ret = uciw_set_value(fullKey, uci_ptr_value);
        } 
      }
      else 
      if (sameSectionCount <= atoi(extractSectionInstance)) /* extractSectionInstance is 0-based */
      {
      /***** create new section ******/
        newSectionCount = atoi(extractSectionInstance) - sameSectionCount + 1;
        //printf("\n[%s,%d]newSectionCount:%d",__FUNCTION__,__LINE__,newSectionCount);
        for (newSectionIndex=0;newSectionIndex<newSectionCount;newSectionIndex++)
        {
          ret = uciw_add_section(uci_ptr_package, extractSectionType);
          if (ret != UCI_OK)
            break;
        }
        if (ret == UCI_OK) /* create option */
        {
          sprintf(fullKey,"%s.%s.%s",p_ptr->package,p_ptr->section,p_ptr->option);
          //printf("\n[%s,%d]key:%s,value:%s",__FUNCTION__,__LINE__,fullKey,uci_ptr_value);
          ret = uciw_set_value(fullKey, uci_ptr_value);
        }
      }
    }
    else /* the format of "<package>.<section>" */
    {
      sprintf(fullKey,"%s.%s",p_ptr->package,p_ptr->section);
      //printf("\n[%s,%d]key:%s,value:%s",__FUNCTION__,__LINE__,fullKey,uci_ptr_value);
      ret = uciw_set_value(fullKey, uci_ptr_value);
    }
  }
  else
  {
    /*At first, create section  */
    sprintf(fullKey,"%s.%s",p_ptr->package,p_ptr->section);
    //printf("\n[%s,%d]key:%s,value:%s",__FUNCTION__,__LINE__,fullKey,sType);
    ret = uciw_set_value(fullKey, sType);

    /*Second, create option */
    if (ret == UCI_OK)
    {
      sprintf(fullKey,"%s.%s.%s",p_ptr->package,p_ptr->section,p_ptr->option);
      //printf("\n[%s,%d]key:%s,value:%s",__FUNCTION__,__LINE__,fullKey,uci_ptr_value);
      ret = uciw_set_value(fullKey, uci_ptr_value);
    }
  }
  return ret;

}

/*
 **************************************
 * F1 UCI interface 
 **************************************
 */
 
void uciw_dump(char* package)
{
  char **configs = NULL;
  char **p;

  if (package == NULL) /* show all package */
  {
    if ((uci_list_configs(ctx, &configs) != UCI_OK) || !configs) 
    {
      cli_perror();
      return ;
    }

    for (p = configs; *p; p++) 
    {
      uci_dump_package(*p);
    }
  }
  else
  {
    uci_dump_package(package);
  }
}

int uciw_get_value(char* key,char* value,int valueLen)
{
  struct uci_element *e;
  struct uci_ptr ptr;
  bool sep = false;
  //int ret = UCI_OK;
  
  if ((ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

  //printf("\n\r[%s,%d]key:%s",__FUNCTION__,__LINE__,key);
  //fflush(stdout);
  if (uci_lookup_ptr(ctx, &ptr, key, true) != UCI_OK) 
  {
    cli_perror();
    return ctx->err; //UCI_ERR_NOTFOUND;
  }

  
  if (!(ptr.flags & UCI_LOOKUP_COMPLETE)) /* key not found */
  {
    ctx->err = UCI_ERR_NOTFOUND;
    //cli_perror();
    return UCI_ERR_NOTFOUND;
  }

  //printf("\n\r[%s,%d]",__FUNCTION__,__LINE__);  
  memset(value, 0, valueLen);
  e = ptr.last;
  switch(e->type) 
  {
  case UCI_TYPE_SECTION:
    //printf("%s\n\r", ptr.s->type);
    strncpy(value,ptr.s->type,(size_t)valueLen);
    value[valueLen-1]='\0';
    break;
  case UCI_TYPE_OPTION:
    switch(ptr.o->type) 
    {
    case UCI_TYPE_STRING:
      //printf("\n\r[%s,%d]value:%s,valueLen:%d\n\r",__FUNCTION__,__LINE__,ptr.o->v.string,valueLen);
      if (strlen(ptr.o->v.string) >= valueLen)
        return UCI_ERR_MEM;
      strncpy(value,ptr.o->v.string,valueLen);
      value[valueLen-1]='\0';
      break;
    case UCI_TYPE_LIST:
      uci_foreach_element(&ptr.o->v.list, e) 
      {
        //printf("%s%s", (sep ? delimiter : ""), e->name);
        if (strlen(value) + strlen(e->name) + 1 >= valueLen)
          return UCI_ERR_MEM; 
        sprintf(value,"%s%s%s",value,(sep ? delimiter : ""), e->name);
        sep = true;
      }
      //printf("\n\r");
      break;
    default:
      printf("<unknown>\n\r");
      break;
    }
    break;
  default:
    break;
  }

  return UCI_OK;
}

int uciw_set_value(char* key, char* value)
{
  struct uci_ptr ptr;
  char  param[MAX_MAX_KEY_SIZE];
  int ret = UCI_OK;
  char   changedSection[MEDIUM_KEY_SIZE];
  struct uci_ptr ptrChange; 
  struct uci_ptr ptrChange2; 
  int retTrace;
  char* atSymbol=NULL;
  char keyWithSectionOption[MEDIUM_KEY_SIZE]="\0";
  

  if ((ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

/* jack tseng 08/23/2010 add for recording the section type and index when the key has no section name */
#if 1
  atSymbol=strstr(key,"@");
  //printf("\n\r[%s,%d]atSymbol:%s\n\r",__FUNCTION__,__LINE__,atSymbol);  
  if (atSymbol != NULL) /* the key format will be a.@b[1].c */
  {
    strcpy(keyWithSectionOption, atSymbol);
  }
#endif
/* end of jack tseng 08/23/2010 */


  sprintf(param,"%s=%s",key,value);
  //printf("\n\r[%s,%d]param:%s\n\r",__FUNCTION__,__LINE__,param);  
  if (uci_lookup_ptr(ctx, &ptr, param, true) != UCI_OK) 
  {
    cli_perror();
    return ctx->err; //UCI_ERR_NOTFOUND;
  }

  
  ret = uci_set(ctx, &ptr);
/* jack tseng 12/16/2009 modification  */
/* We don't have to save the changed in the file under "/tmp/.uci"  */
#if 0
  if (ret == UCI_OK)
  {
    ret = uci_save(ctx, ptr.p); /* save changes */
  }

  if (ret != UCI_OK) 
  {
    cli_perror();
    return UCI_ERR_MEM;
  }
#endif  
/* end of jack tseng 12/16/2009 */
  
  //printf("\n uciw_set_value ret:%d",ret);

  /* save the list of changed key into <package name>.change.key */
/* jack tseng 03/18/2010 */
/* only check if return value is UCI_OK */
#if 1
  if (ret==UCI_OK)
#else
  if ( (ret==UCI_OK) && ptr.s && ptr.o)
#endif    
  {
    validate_trace_file(ptr.p->e.name,false);  /* ignore return */
    #if 1 /* jack tseng 08/23/2010 add for recording the section type and index when the key has no section name */
    if (strlen(keyWithSectionOption)>0)
      sprintf(changedSection, "%s_%s.%s=%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, keyWithSectionOption);
    else if (ptr.o)
      sprintf(changedSection, "%s_%s.%s=%s.%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name,ptr.o->e.name);
    else
      sprintf(changedSection, "%s_%s.%s=%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name);
    #else
    if (ptr.o)
      sprintf(changedSection, "%s_%s.%s=%s.%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name,ptr.o->e.name);
    else
      sprintf(changedSection, "%s_%s.%s=%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name);
    #endif

    //printf("\n\rjack changedSection:%s\n\r",changedSection);
    if (uci_lookup_ptr(trace_ctx, &ptrChange, changedSection, true) != UCI_OK) 
    {
    /* the file is exist, but the section is not exist. Create it again */
      validate_trace_file(ptr.p->e.name,true);  /* ignore return */ 
    if (uci_lookup_ptr(trace_ctx, &ptrChange, changedSection, true) != UCI_OK) 
    {
      cli_perror();
      return UCI_ERR_NOTFOUND;
    }
    }
    //printf("\n\r[%s,%d]\n\r",__FUNCTION__,__LINE__);
    retTrace = uci_add_list(trace_ctx, &ptrChange);

    if (retTrace != UCI_OK) /* rewirte again */
        {
      //printf("\n\r[%s,%d]\n\r",__FUNCTION__,__LINE__);
      validate_trace_file(ptr.p->e.name,true);  /* ignore return */ 
      
      if (strlen(keyWithSectionOption)>0) /* jack tseng 08/23/2010 add for recording the section type and index when the key has no section name */
        sprintf(changedSection, "%s_%s.%s=%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, keyWithSectionOption);
      else if (ptr.o)
        sprintf(changedSection, "%s_%s.%s=%s.%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name,ptr.o->e.name);
      else
        sprintf(changedSection, "%s_%s.%s=%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name);

      ret = uci_lookup_ptr(trace_ctx, &ptrChange2, changedSection, true);
      //printf("\n\r[%s,%d]ret:%d\n\r",__FUNCTION__,__LINE__,ret);
    #if 1
      retTrace = uci_set(trace_ctx, &ptrChange2);
    #else
      retTrace = uci_add_list(trace_ctx, &ptrChange2);
    #endif
      //printf("\n\r[%s,%d]retTrace:%d\n\r",__FUNCTION__,__LINE__,retTrace);
      if (retTrace != UCI_OK)
        ret = UCI_TRACE_SAVE_FAIL;
    }
  }
  
  return ret;
}

/* jack tseng 03/17/2010 add: */
/* For moto request, if the package name, section name or option name is not exist, create it and set it */
int uciw_set_new_value(char* key,char* sectionType, char* value)
{
  struct uci_ptr ptr;
  char  param[MAX_MAX_KEY_SIZE];
  int ret = UCI_OK;
  char   changedSection[MEDIUM_KEY_SIZE];
  struct uci_ptr ptrChange; 
  int retTrace;
  char* atSymbol=NULL;
  char keyWithSectionOption[MEDIUM_KEY_SIZE]="\0";

  if ((ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

/* jack tseng 08/23/2010 add for recording the section type and index when the key has no section name */
#if 1
  atSymbol=strstr(key,"@");
  //printf("\n\r[%s,%d]atSymbol:%s\n\r",__FUNCTION__,__LINE__,atSymbol);  
  if (atSymbol != NULL) /* the key format will be a.@b[1].c */
  {
    strcpy(keyWithSectionOption, atSymbol);
  }
#endif
/* end of jack tseng 08/23/2010 */  

  sprintf(param,"%s=%s",key,value);
  if (uci_lookup_ptr(ctx, &ptr, param, true) != UCI_OK) 
  {
    //printf("\n\rpackage:%s,section:%s,option:%s",ptr.package,ptr.section,ptr.option);
    //fflush(stdout);
    
    /* the key is not found, we will create it. */
    /* If package is not exist, create the file */
    /* If section or option is not exist, create it in the package file */
    if ((ptr.package != NULL) && (ptr.section != NULL))
    {
      ret = create_new_key(&ptr,sectionType);
      return ret;
    }
    else
    {
      cli_perror();
      return ctx->err; //UCI_ERR_NOTFOUND;
    }
  }
  
  ret = uci_set(ctx, &ptr);
  
  //printf("\n[%s,%d] uciw_set_value ret:%d,error:%d",__FUNCTION__,__LINE__,ret,ctx->err);

  /* save the list of changed key into <package name>.change.key */
  if ( ret==UCI_OK)
  {
    validate_trace_file(ptr.p->e.name,false);  /* ignore return */
    #if 1
    if (strlen(keyWithSectionOption)>0)
      sprintf(changedSection, "%s_%s.%s=%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, keyWithSectionOption);
    else if (ptr.o)
      sprintf(changedSection, "%s_%s.%s=%s.%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name,ptr.o->e.name);
    else /* don't contain option */
      sprintf(changedSection, "%s_%s.%s=%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name);
    #else
    if (ptr.o)
    sprintf(changedSection, "%s_%s.%s=%s.%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name,ptr.o->e.name);
    else /* don't contain option */
      sprintf(changedSection, "%s_%s.%s=%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name);
    #endif
    //printf("\n\rchangedSection:%s\n\r",changedSection);
    if (uci_lookup_ptr(trace_ctx, &ptrChange, changedSection, true) != UCI_OK) 
    {
      cli_perror();
      return UCI_ERR_NOTFOUND;
    }
    retTrace = uci_add_list(trace_ctx, &ptrChange);
    if (retTrace != UCI_OK) /* rewirte again */
    {
      validate_trace_file(ptr.p->e.name,true);  /* ignore return */ 
      retTrace = uci_add_list(trace_ctx, &ptrChange);
      if (retTrace != UCI_OK)
        ret = UCI_TRACE_SAVE_FAIL;
    }
  }
  else
  { 
    if ((ptr.package != NULL) && (ptr.section != NULL))
    {
      ret = create_new_key(&ptr,sectionType);
    }
  }
  
  return ret;
}
/* end of jack tseng 03/17/2010 */

int uciw_delete_key(char* key)
{
  struct uci_ptr ptr;
  int ret = UCI_OK;
  char backupKey[MAX_KEY_SIZE];
  char   changedSection[MAX_KEY_SIZE];
  struct uci_ptr ptrChange;
  int retTrace;

  if ((ctx==NULL) || (key == NULL))
  {
    return UCI_ERR_INVAL;
  }

  strcpy(backupKey, key);
  
  if (uci_lookup_ptr(ctx, &ptr, key, true) != UCI_OK) 
  {
    cli_perror();
    return ctx->err; //UCI_ERR_NOTFOUND;
  }

  ret = uci_delete(ctx, &ptr);

/* jack tseng 12/16/2009 modification  */
/* We don't have to save the changed in the file under "/tmp/.uci"  */
#if 0
  if (ret == UCI_OK)
  {
    ret = uci_save(ctx, ptr.p); /* save changes */
  }

  if (ret != UCI_OK) 
  {
    cli_perror();
    return UCI_ERR_MEM;
  }
#endif
/* end of jack tseng 12/16/2009 */  


  /* save the list of changed key into <package name>.change.key */
  if ( ret==UCI_OK)
  {
    validate_trace_file(ptr.p->e.name,false);  /* ignore return */
    #if 1
    sprintf(changedSection, "%s_%s.%s=-%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey,backupKey);
    #else
    sprintf(changedSection, "%s.%s=-%s",ptr.p->e.name,changeSectionKey,backupKey);
    #endif
    //printf("\n\r[%s,%d]changedSection:%s\n\r",__FUNCTION__,__LINE__,changedSection);

    if (uci_lookup_ptr(trace_ctx, &ptrChange, changedSection, true) != UCI_OK) 
    {
      cli_perror();
      return UCI_ERR_NOTFOUND;
    }
    retTrace = uci_add_list(trace_ctx, &ptrChange);
    if (retTrace != UCI_OK) /* rewirte again */
    {
      validate_trace_file(ptr.p->e.name,true);  /* ignore return */ 
      retTrace = uci_add_list(trace_ctx, &ptrChange);
      if (retTrace != UCI_OK)
        ret = UCI_TRACE_SAVE_FAIL;
    }
  } 

  return ret;
}

/* uciw_delete_section() is the same as uciw_delete_key() */
int uciw_delete_section(char* section)
{
  return uciw_delete_key(section);
}

int uciw_commit(char* package)
{
  //char **configs = NULL;
  // Added by James Chen, Foxconn, 2010/08/26
#if 1
  char *p;
  glob_t globbuf;
  int i;
  char *dir;
#else
  char **p;
#endif
    // End of Added by James Chen, Foxconn, 2010/08/26
  int ret;

  if (ctx==NULL)
  {
    return UCI_ERR_INVAL;
  }

/* Before commit the change in memory to the associated file, we must lock the file "tlock" first */
/* Each time, we only allow only one commit for a package or all packages. */ 

/* jack tseng 12/17/2009 */
#if 0
  grant_trans_lock();
#endif
/* end of jack tseng 01/14/2010 */    

  if (package == NULL) /* commit all packages */
  {
      // Modified by James Chen, Foxconn, 2010/08/26
      // uci_list_configs() causes memory leak
      // We use our own way to get the list of files in the directory.
#if 1
      dir = uci_malloc(ctx, strlen(ctx->confdir) + 1 + sizeof("/*"));
      sprintf(dir, "%s/*", ctx->confdir);
      if (glob(dir, GLOB_MARK, NULL, &globbuf) != 0) {
        free(dir);
        return UCI_ERR_NOTFOUND;
      }
      for(i = 0; i < globbuf.gl_pathc; i++) {
        p = strrchr(globbuf.gl_pathv[i], '/');
          p++;
        if (!*p)
          continue;
        ret = uci_commit_package_do(p);
      }
      free(dir);
      globfree(&globbuf);
#else
    if ((uci_list_configs(ctx, &configs) != UCI_OK) || !configs) 
    {
      cli_perror();
      return ctx->err;
    }

    for (p = configs; *p; p++) 
    {
      //printf("\n\r commit package %s",*p);    
      ret = uci_commit_package_do(*p);
    }
#endif
      // End of Modified by James Chen, Foxconn, 2010/08/26
  }
  else /* commit the specified package */
  {
    ret = uci_commit_package_do(package);
  #if 0
    if (ret == UCI_ERR_IO) /* the package file is blocking, retrying.... */
    {
      usleep(5000); /* microseconds*/
    

    }
  #endif  
      
  }

/* jack tseng 12/17/2009 */
#if 0
  release_trans_lock();
#endif
/* end of jack tseng 01/14/2010 */
  
  return ret;
  
}

/* jack tseng 11/17/2010 add for reset to default */
/* Ignore the change list of the package  */
int uciw_commit_force(char* package)
{
  //char **configs = NULL;
  char *p;
  glob_t globbuf;
  int i;
  char *dir;
  int ret;

  if (ctx==NULL)
  {
    return UCI_ERR_INVAL;
  }


  if (package == NULL) /* commit all packages */
  {
      dir = uci_malloc(ctx, strlen(ctx->confdir) + 1 + sizeof("/*"));
      sprintf(dir, "%s/*", ctx->confdir);
      if (glob(dir, GLOB_MARK, NULL, &globbuf) != 0) {
        free(dir);
        return UCI_ERR_NOTFOUND;
      }
      for(i = 0; i < globbuf.gl_pathc; i++) {
        p = strrchr(globbuf.gl_pathv[i], '/');
          p++;
        if (!*p)
          continue;
        uci_commit_package_do_force(p);
      }
      free(dir);
      globfree(&globbuf);
  }
  else /* commit the specified package */
  {
    ret = uci_commit_package_do_force(package);
      
  }

  return ret;
  
}
/* end of jack tseng 11/16/2010 */


int uciw_add_value(char* key, char* value)
{
  struct uci_ptr ptr;
  char  param[MAX_MAX_KEY_SIZE];
  int ret = UCI_OK;
  char   changedSection[MAX_KEY_SIZE];
  struct uci_ptr ptrChange;
  int retTrace;
  char* atSymbol=NULL;
  char keyWithSectionOption[MEDIUM_KEY_SIZE]="\0";

  

  if ((ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

/* jack tseng 08/23/2010 add for recording the section type and index when the key has no section name */
#if 1
  atSymbol=strstr(key,"@");
  //printf("\n\r[%s,%d]atSymbol:%s\n\r",__FUNCTION__,__LINE__,atSymbol);  
  if (atSymbol != NULL) /* the key format will be a.@b[1].c */
  {
    strcpy(keyWithSectionOption, atSymbol);
  }
#endif
/* end of jack tseng 08/23/2010 */  

  sprintf(param,"%s=%s",key,value);
  if (uci_lookup_ptr(ctx, &ptr, param, true) != UCI_OK) 
  {
    cli_perror();
    return ctx->err; //UCI_ERR_NOTFOUND;
  }
  ret = uci_add_list(ctx, &ptr);

/* jack tseng 12/16/2009 modification  */
/* We don't have to save the changed in the file under "/tmp/.uci"  */
#if 0 
  if (ret == UCI_OK)
  {
    ret = uci_save(ctx, ptr.p); /* save changes */
  }
#endif
/* end of jack tseng 12/16/2009 */

  /* save the list of changed key into <package name>.change.key */
  if ( (ret==UCI_OK) && ptr.s && ptr.o)
  {
    validate_trace_file(ptr.p->e.name,false);  /* ignore return */
    
    #if 1
    if (strlen(keyWithSectionOption)>0) /* jack tseng 08/23/2010 add for recording the section type and index when the key has no section name */
      sprintf(changedSection, "%s_%s.%s=|%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, keyWithSectionOption);
    else
    sprintf(changedSection, "%s_%s.%s=|%s.%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey, ptr.s->e.name,ptr.o->e.name);
    #else
    sprintf(changedSection, "%s.%s=|%s.%s",ptr.p->e.name,changeSectionKey, ptr.s->e.name,ptr.o->e.name);
    #endif
    //printf("\n\r[%s,%d]changedSection:%s\n\r",__FUNCTION__,__LINE__,changedSection);

    if (uci_lookup_ptr(trace_ctx, &ptrChange, changedSection, true) != UCI_OK) 
    {
      cli_perror();
      return UCI_ERR_NOTFOUND;
    }
    retTrace = uci_add_list(trace_ctx, &ptrChange);
    if (retTrace != UCI_OK) /* rewirte again */
    {
      validate_trace_file(ptr.p->e.name,true);  /* ignore return */ 
      retTrace = uci_add_list(trace_ctx, &ptrChange);
      if (retTrace != UCI_OK)
        ret = UCI_TRACE_SAVE_FAIL;
    }
  }
  return ret;
}

/* jack tseng 02/04/2010 add */
/* For moto request, uciw_delete_value will delete the value of the specified key */
int uciw_delete_value(char* key, char* value)
{
  //struct uci_ptr ptr;
  char content[MAX_KEY_SIZE];
  char modifiedContent[MAX_KEY_SIZE];
  char backupKey[LARGE_KEY_SIZE];
  char *valuePosStart=NULL,*valuePosEnd=NULL;
  //char  param[MAX_KEY_SIZE];
  int ret = UCI_OK;
  //char   changedSection[MAX_KEY_SIZE];
  //struct uci_ptr ptrChange;
  

  if ((ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

  strcpy(backupKey, key);
/* step 1: get the content of the key. The content can be multiple values separated by " " */
  ret = uciw_get_value(key, content, MAX_KEY_SIZE);
  if (ret == UCI_OK)
  {
/* step 2: delete the input varaibe 'value' from the content */
    if ((valuePosStart=strstr(content, value)) != NULL ) /* find the input value */
    {
      if (strlen(content)==strlen(value)) /* the same string and the content will be empty */
      {
        modifiedContent[0] = '\0';
      }
      else
      {
        valuePosEnd = valuePosStart+strlen(value);
        if (valuePosStart != content) /* Not the begin of the content */
        {
          valuePosStart--; /*  igonre the " "  */
        } 
        *valuePosStart = '\0';
        
        if (*valuePosEnd == '\0') /* reach the end of the conent */
        {
          strcpy(modifiedContent, content);
        }
        else
        {
          sprintf(modifiedContent, "%s%s",content, valuePosEnd);
        }
        
      }
/* step 3: write the modified content into the key */   
      /* ignore the starting space characters */
      valuePosStart = modifiedContent;
      while (*valuePosStart == ' ')
        valuePosStart++;
      ret = uciw_set_value(backupKey, valuePosStart); /* This function will also save the change */
    }
  }
  

  return ret;
}

int uciw_get_changes(char* package, char* value,int valueLen)
{
  char   changedKey[MAX_KEY_SIZE];
  struct uci_ptr ptr;

#if 1 /* use another context to record the change */
  if ((trace_ctx==NULL) || (package == NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }
  #if 1
  sprintf(changedKey, "%s_%s.%s",package,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionLastKey);
  #else
  sprintf(changedKey, "%s.%s",package,changeSectionLastKey);
  #endif
  //printf("\n[%s,%d]changedSection:%s,len:%d",__FUNCTION__,__LINE__,changedKey,strlen(changedKey));
  //fflush(stdout);

  if (uci_lookup_ptr(trace_ctx, &ptr, changedKey, true) != UCI_OK) 
  {
    cli_perror();
    return trace_ctx->err; //UCI_ERR_NOTFOUND;
  }

  if (!(ptr.flags & UCI_LOOKUP_COMPLETE)) /* key not found */
  {
    trace_ctx->err = UCI_ERR_NOTFOUND;
    cli_perror();
    return UCI_ERR_NOTFOUND;
  }

  if(ptr.o->type == UCI_TYPE_STRING) 
  {
    //printf("\n\r[%s,%d]changed value:%s\n\r",__FUNCTION__,__LINE__,ptr.o->v.string);
    memset(value, 0, valueLen);
    if (strlen(ptr.o->v.string) >= valueLen)
      return UCI_ERR_MEM;
    strncpy(value,ptr.o->v.string,valueLen);
    value[valueLen-1]='\0';
  }
  
  return UCI_OK;
#else
  char* changedKeyValue=NULL;

  //printf("\n[%s,%d]package:%s\n",__FUNCTION__,__LINE__,package);
  if ((ctx==NULL) || (package == NULL))
  {
    return NULL;
  }

  sprintf(changedKey, "%s.%s",package,changeSectionLastKey);
  printf("\n[%s,%d]changedSection:%s\n",__FUNCTION__,__LINE__,changedKey);

  if (uci_lookup_ptr(ctx, &ptr, changedKey, true) != UCI_OK) 
  {
    cli_perror();
    return NULL;
  }
  
  if (!(ptr.flags & UCI_LOOKUP_COMPLETE)) /* key not found */
  {
    ctx->err = UCI_ERR_NOTFOUND;
    cli_perror();
    return NULL;
  }

  if(ptr.o->type == UCI_TYPE_STRING) 
  {
    printf("\n[%s,%d]changed value:%s\n",__FUNCTION__,__LINE__,ptr.o->v.string);
    changedKeyValue = strdup(ptr.o->v.string);
  }
  
  return changedKeyValue;
#endif  
}

int uciw_update(char* packageName)
{
  //char  packageFullName[FILENAME_MAX];
  struct uci_ptr ptr;
  char traceFile[64];
    
/* jack tseng 12/29/2009 modification */
/* Allow that the package name is NULL. If the package name is NULL, update all packages */
  struct uci_package *p;
  struct uci_element *e, *tmp;
  if (ctx==NULL)
  {
    return UCI_ERR_INVAL;
  }

  if (packageName == NULL) /* unload all packages */
  {
    uci_foreach_element_safe(&ctx->root, tmp, e) 
    { 
      p = uci_to_package(e);
      //printf("\n package:%s",p->e.name);
      uci_unload(ctx, p);
    }
  }
  else
  {
    if (uci_lookup_ptr(ctx, &ptr, packageName, true) == UCI_OK) 
    {
      uci_unload(ctx, ptr.p);
    }
    else
    {
      cli_perror();
      return ctx->err;
    }
  }

/* update trace_ctx */
  if (trace_ctx==NULL)
  {
    return UCI_ERR_INVAL;
  }

  if (packageName == NULL) /* unload all packages */
  {
    uci_foreach_element_safe(&trace_ctx->root, tmp, e) 
    { 
      p = uci_to_package(e);
      //printf("\n package:%s for trace_ctx",p->e.name);
      uci_unload(trace_ctx, p);
    }
  }
  else
  {
    sprintf(traceFile,"%s_%s",  packageName,UCI_CHANGE_TRACE_FILE_DELTA);
    //if (uci_lookup_ptr(trace_ctx, &ptr, packageName, true) == UCI_OK) 
    if (uci_lookup_ptr(trace_ctx, &ptr, traceFile, true) == UCI_OK) 
    {
      //printf("\n package:%s for trace_ctx",packageName);
      uci_unload(trace_ctx, ptr.p);
    }
    else
    {
      cli_perror();
      return trace_ctx->err;
    }
  }
  

  #if 0
  if (ctx->confdir)
  {
    sprintf(packageFullName, ctx->confdir,packageName);
    printf("\n[%s,%d]packageFullName:%s",__FUNCTION__,__LINE__,packageFullName);
  }
  
/* reload the package */  
  //if (uci_load(ctx, packageName, NULL) != UCI_OK)
  if (uci_load(ctx, packageFullName, NULL) != UCI_OK) 
  {
    return UCI_ERR_NOTFOUND;
  }
  #endif
  return UCI_OK;
}

int uciw_import_file(char* importFilename, char* packageName)
{
  struct uci_package *package = NULL;
  //char *name = NULL;
  int ret = UCI_OK;
  FILE* fileStream=NULL;
  bool merge = true;

  //printf("\n\r[%s,%d]",__FUNCTION__,__LINE__);

  if ((ctx==NULL) || (importFilename == NULL))
  {
    return UCI_ERR_INVAL;
  }
  
  if (packageName) /* if packageName is not NULL, the import data wil be merged to the package wit the packgeName */
  {
    merge = true;
  }
  
  if (merge)
  {
    if (uci_load(ctx, packageName, &package) != UCI_OK)
    {
      package = NULL;
    }
  }
  
  fileStream = fopen(importFilename, "r");
  
  if (fileStream == NULL)
  {
    return UCI_ERR_MEM;
  }
  ret = uci_import(ctx, fileStream, packageName, &package, (packageName != NULL));

  if (ret == UCI_OK) 
  {
    if (merge) 
    {
    /* jack tseng 12/16/2009 modification  */
    /* We don't have to save the changed in the file under "/tmp/.uci"  */
    #if 0
      ret = uci_save(ctx, package);
    #endif 
    /* end of jack tseng 12/16/2009 */
    
      #if 1
      if (ret == UCI_OK)
      {
        ret = uci_commit(ctx, &package, false);
      }
      #endif
    } 
    else 
    {
      struct uci_element *e;
      /* loop through all config sections and overwrite existing data */
      uci_foreach_element(&ctx->root, e) 
      {
        struct uci_package *p = uci_to_package(e);
        ret = uci_commit(ctx, &p, true);
      }
    }
  }
  
  fclose(fileStream);
  
  //printf("\n\r[%s,%d]",__FUNCTION__,__LINE__);
  if (ret != UCI_OK) 
  {
    cli_perror();
    return ctx->err; //UCI_ERR_MEM;
  }
  return ret;
}

/* jack tseng 09/06/2010 add for import package */
/* merge the uci file "file" in "directory" to the "package" */
int uciw_merge_file(char* directory, char* file, char* package)
{
  struct uci_element *e,*eSection,*eOption, *eValue;
  struct uci_package *p;
  struct uci_option *o;
  struct uci_ptr ptr;
  char origConfigDir[MAX_KEY_SIZE];
  char key[MAX_KEY_SIZE];
  char  valueList[LARGE_KEY_SIZE];
  struct uci_merge_list *mergeListHead = NULL;
  struct uci_merge_list *mergeItem;
  struct uci_merge_list *mergeTemp;
  char fullName[LARGE_KEY_SIZE];
  struct stat sb;
  int fd;
  int ret=UCI_OK;
  //char sectionType[LARGE_KEY_SIZE];
  char value[MAX_KEY_SIZE];
  
  
  strcpy(origConfigDir, ctx->confdir); /* store the original config directory */
  
  uciw_set_config_dir(directory);
  uciw_update(file);
    
  if (uci_lookup_ptr(ctx, &ptr, file, true) != UCI_OK) 
  {
    if (ctx->err > 0)
    {
      printf("\n\r[uci_dump_package]errno:%d\n\r",ctx->err);
      fflush(stdout);
    }
      
    cli_perror();
    return ctx->err;
  }

  e = ptr.last;
  if (e->type == UCI_TYPE_PACKAGE) 
  {
    p = ptr.p;
    uci_reset_typelist();
    uci_foreach_element( &p->sections, eSection) 
    {
      struct uci_section *s = uci_to_section(eSection);
      cur_section_ref = uci_lookup_section_ref(s);
    
      //uci_show_section(s);
      
      const char *cname;
      const char *sname;

      cname = s->package->e.name;
      sname = (cur_section_ref ? cur_section_ref : s->e.name);
      
      //printf("%s.%s=%s\n", cname, sname, s->type);
      /* insert the key to the merged list */
      if (mergeListHead == NULL)
      {
        mergeListHead = (struct uci_merge_list *)malloc(sizeof(struct uci_merge_list)); 
        memset(mergeListHead, 0, sizeof(struct uci_merge_list));
        mergeItem = mergeListHead;
      }
      else
      {
        if (mergeItem->next == NULL)
        {
          mergeItem->next = (struct uci_merge_list *)malloc(sizeof(struct uci_merge_list)); 
          mergeItem = mergeItem->next;
          memset(mergeItem, 0, sizeof(struct uci_merge_list));
        }
      }
      sprintf(key, "%s.%s", cname, sname);
      mergeItem->name = strdup(key);
      mergeItem->value = strdup(s->type);
      mergeItem->type = TYPE_SECTION;
      //mergeItem = mergeItem->next;
      
      uci_foreach_element(&s->options, eOption) 
      {
        o = uci_to_option(eOption);
        //printf("%s.%s.%s=",
        //o->section->package->e.name,
        //(cur_section_ref ? cur_section_ref : o->section->e.name),
        //  o->e.name);
        
        /* insert the key to the merged list */
        if (mergeItem->next == NULL)
        {
          mergeItem->next = (struct uci_merge_list *)malloc(sizeof(struct uci_merge_list)); 
          mergeItem = mergeItem->next;
          memset(mergeItem, 0, sizeof(struct uci_merge_list));
        }
        sprintf(key, "%s.%s.%s",
        o->section->package->e.name,
        (cur_section_ref ? cur_section_ref : o->section->e.name),
          o->e.name);
        mergeItem->name = strdup(key);
        mergeItem->type = TYPE_OPTION; 
        
        bool sep = false;
        switch(o->type) {
        case UCI_TYPE_STRING:
          printf("%s\n", o->v.string);
          mergeItem->value = strdup(o->v.string);         
          break;
        case UCI_TYPE_LIST:
          valueList[0] = '\0'; 
          uci_foreach_element(&o->v.list, eValue) {
          //printf("%s%s", (sep ? delimiter : ""), eValue->name);
          sprintf(valueList,"%s%s%s", valueList,(sep ? delimiter : ""), eValue->name);
          sep = true;
          }
          mergeItem->value = strdup(valueList);
          //printf("\n");
        break;
        default:
          printf("<unknown>\n");
        break;
        }
      }
    
    }
    uci_reset_typelist();
  }



/* set the value of the merged list to the file in /etc/config/general */
  uciw_set_config_dir(origConfigDir);
  uciw_update(file);
  
/* check if the file is exist */
  sprintf(fullName,"%s/%s",ctx->confdir,file);
  if (stat(fullName, &sb) == -1)  /* file is not exist, create it */
    {
          fd = open(fullName, O_CREAT, S_IRWXU|S_IRWXG|S_IROTH | S_IWOTH);
    if (fd)
      close(fd);
  } 
  
  mergeItem = mergeListHead;
  while(mergeItem != NULL)
  {
    //printf("name:%s, value:%s\n",mergeItem->name,mergeItem->value);
    /* check */
  #if 1
    if (mergeItem->type == TYPE_SECTION)
    {
      strcpy(key,mergeItem->name);
      ret = uciw_get_value(key, value, MAX_KEY_SIZE);
      if ( (ret != UCI_OK) && (strstr(mergeItem->name,"@") ) )
        uciw_add_section(file, mergeItem->value);
      else
    uciw_set_value(mergeItem->name, mergeItem->value);
      
    }
    else
    {
      uciw_set_value(mergeItem->name, mergeItem->value);    
    }
    
  #else
    if (mergeItem->type == TYPE_SECTION)
    {
      strcpy(sectionType, mergeItem->value);
      printf("sectionType:%s\n",mergeItem->value);
    }
    printf("sectionType:%s\n",mergeItem->value);
    ret = uciw_set_new_value(mergeItem->name, sectionType, mergeItem->value);
  #endif  
    mergeItem= mergeItem->next;
  }
/* jack tseng 11/16/2010 modification for reset-to-default */
#if 1
  ret = uciw_commit_force(file);
#else
  uciw_commit(file);
#endif


/* Free the merged list */
  mergeItem = mergeListHead;
  while(mergeItem != NULL)
  {
    mergeTemp = mergeItem;
    mergeItem= mergeItem->next;
    free(mergeTemp->name);
    free(mergeTemp->value);
    free(mergeTemp);
  }

  return ret;
}


int uciw_export_file(char* exportFilename, char* packageName)
{
  struct uci_ptr ptr;
  int ret = UCI_OK;
  FILE* fileStream;
  char **configs = NULL;
  char **p;

  if ((ctx==NULL) || (exportFilename == NULL))
  {
    return UCI_ERR_INVAL;
  }
  
  

  fileStream = fopen(exportFilename, "w");  
  if (fileStream == NULL)
  {
    return UCI_ERR_MEM;
  }

  if (packageName == NULL) /* export all package */
  {
    if ((uci_list_configs(ctx, &configs) != UCI_OK) || !configs) 
    {
      cli_perror();
      return ctx->err; //UCI_ERR_NOTFOUND;
    }

    for (p = configs; *p; p++) 
    {
      if (uci_lookup_ptr(ctx, &ptr, *p, true) != UCI_OK) 
      {
        cli_perror();
        return ctx->err; //UCI_ERR_NOTFOUND;
      }
      ret = uci_export(ctx, fileStream, ptr.p, true);
    }
  }
  else
  {
    if (uci_lookup_ptr(ctx, &ptr, packageName, true) != UCI_OK) 
    {
      cli_perror();
      return ctx->err; //UCI_ERR_NOTFOUND;
    }
    ret = uci_export(ctx, fileStream, ptr.p, true);
  } 
  

  fclose(fileStream);

  if (ret != UCI_OK)
    return ctx->err;
  else
    return ret;
}

int uciw_add_section(char* packageName,char* setionType)
{
  //struct uci_package *p = NULL;
  struct uci_section *s = NULL;
  int ret;
  struct uci_ptr ptr;
  char   changedSection[MAX_KEY_SIZE];
  struct uci_ptr ptrChange;
  int retTrace;

  if ((ctx==NULL) || (packageName == NULL) || (setionType == NULL) )
  {
    return UCI_ERR_INVAL;
  }
  
  if (uci_lookup_ptr(ctx, &ptr, packageName, true) != UCI_OK)   
  {
    return ctx->err; //UCI_ERR_NOTFOUND;;
  }
  ret = uci_add_section(ctx, ptr.p, setionType, &s);
  
/* jack tseng 12/16/2009 modification  */
/* We don't have to save the changed in the file under "/tmp/.uci"  */
#if 0
  if (ret == UCI_OK)
  {
    ret = uci_save(ctx, ptr.p); /* ignore retuen */
  }
#endif 
/* end of jack tseng 12/16/2009 */

  
  //fprintf(stdout, "section:%s,ret:%d\n", s->e.name,ret);

#if 1
  /* save the list of changed key into <package name>.change.key */
  if ( ret==UCI_OK)
  {
    validate_trace_file(ptr.p->e.name,false);  /* ignore return */
    #if 1
    sprintf(changedSection, "%s_%s.%s=+%s",ptr.p->e.name,UCI_CHANGE_TRACE_FILE_DELTA,changeSectionKey,setionType);
    #else
    sprintf(changedSection, "%s.%s=+%s",ptr.p->e.name,changeSectionKey,setionType);
    #endif
    //printf("\n\r[%s,%d]changedSection:%s\n\r",__FUNCTION__,__LINE__,changedSection);

    if (uci_lookup_ptr(trace_ctx, &ptrChange, changedSection, true) != UCI_OK) 
    {
      cli_perror();
      return UCI_ERR_NOTFOUND;
    }
    retTrace = uci_add_list(trace_ctx, &ptrChange);
    if (retTrace != UCI_OK) /* rewirte again */
    {
      validate_trace_file(ptr.p->e.name,true);  /* ignore return */ 
      retTrace = uci_add_list(trace_ctx, &ptrChange);
      if (retTrace != UCI_OK)
        ret = UCI_TRACE_SAVE_FAIL;
    }
  }
#endif

  return ret;
}
  
int uciw_set_config_dir(char* path)
{
  struct stat sb;

  if ((ctx==NULL) || (path == NULL))
  {
    return UCI_ERR_MEM;
  }
/* jack tseng 03/10/2010 add: if the directory is not exist, create it for moto request */
  if (stat(path, &sb) == -1)  /* directory is not exist, create it */
    {
          mkdir(path,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
  }
/* end of jack tseng 03/10/2010 */
  
  uci_set_confdir(ctx, path);

/* jack tseng 03/05/2010 modification: use the same directory with UCI_DEFAULT_DIR */
#if 0
    uci_set_confdir(trace_ctx, path);
#endif
/* end of jack tseng 03/05/2010 */
  
  return UCI_OK;
}

int uciw_init(void)
{
	struct stat sb;

  ctx = uci_alloc_context();
  if (!ctx) 
  {
    fprintf(stderr, "Out of memory\n\r");
    return UCI_ERR_MEM;
  }
      
  uci_set_confdir(ctx, UCI_GENERAL_DIR); /* jack tseng 03/22/2010 add */
  //uci_set_confdir(ctx, UCI_DEFAULT_DIR); /* jack tseng 03/22/2010 add */
      
  //ctx->flags &= ~UCI_FLAG_SAVED_HISTORY;

  trace_ctx = uci_alloc_context();
  if (!trace_ctx) 
  {
    fprintf(stderr, "Out of memory\n\r");
    return UCI_ERR_MEM;
  }
  else
  {
/* jack tseng 06/08/2012 add */
/* check if /tmp/delta is exist */
	if (stat(UCI_CHANGE_TRACE_DIR, &sb) == -1)  /* directory is not exist, create it */
	{
		mkdir(UCI_CHANGE_TRACE_DIR, S_IRWXU|S_IRWXG|S_IROTH | S_IWOTH);
	}
/* end of jack tseng 06/08/2012 */
  
  /* jack tseng 03/17/2010 modification: use the /tmp to store the delta file */
  #if 0
    uci_set_confdir(trace_ctx, UCI_DEFAULT_DIR);
  #else
    uci_set_confdir(trace_ctx, UCI_CHANGE_TRACE_DIR);
  #endif
  }
  
  return UCI_OK;
}

int uciw_exit(void)
{
  if (ctx)
  {
    uci_free_context(ctx);
    ctx = NULL;
  }
  if (trace_ctx)
  {
    uci_free_context(trace_ctx);
    trace_ctx = NULL;
  }
  return UCI_OK;
}

#if 1
int uciw_get_sequence(char* sequence,int len)
{
  int fd;
  unsigned long sec,usec;
  char buf[64];

  /* the sequence is 10 digits */
  if ( (sequence == NULL) || (len < 11 ) )
    return UCI_ERR_MEM;

  //usleep(500);
  fd = open("/proc/uptime",O_RDONLY);

  if (fd == -1)
{
    perror("open");
    return UCI_ERR_IO;
  }
  memset(buf,0,64);
  read(fd,buf,64);
  sscanf(buf,"%lu.%lu",&sec,&usec);
  close(fd);

  sprintf(sequence,"%08ld%ld",sec,usec);
  //printf("\n\rsequence: %s\n\r",sequence);
  return UCI_OK;
}
#endif

/*********************************************************
 * jack tseng 03/22/2010 add
 * A new interface for index handling 
 * Each table will have a option 'index'.
 * In the format of <package name>.@<config type>[index].<option name>,
 * index represents the value of the option 'index'.
 *********************************************************/
int uciw_get_value_by_index(char* key,char* value,int valueLen)
{
  //struct uci_ptr ptr;
  int ret = UCI_OK;
  char package[SMALL_KEY_SIZE]; 
  char section[SMALL_KEY_SIZE];
  char instance[SMALL_KEY_SIZE];
  char option[SMALL_KEY_SIZE]; 
  int sectionEntryPos;
  char fullKey[LARGE_KEY_SIZE];
  
  if ((ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

  /* extract the package name, section name ,option name and instance value from input variable "key" */
  ret = extract_table_token(key, package, section, instance, option);
  //printf("\n\rpackage:%s,section name:%s,option:%s,instance:%s",package,section,option,instance);
  if ( ret == UCI_OK ) /* the multiple instance */
  {
    /* Find the section entry that contains the value of the option "index" to be the same as the variable 'instance' */
    ret = find_table_index(package, section, instance, &sectionEntryPos);
    //printf("\n\rret:%d,sectionEntryPos:%d",ret,sectionEntryPos);    
    if (ret == UCI_OK)
    {
      if (strlen(option)>0)
        sprintf(fullKey,"%s.@%s[%d].%s",package,section,sectionEntryPos,option);
      else
        sprintf(fullKey,"%s.@%s[%d]",package,section,sectionEntryPos);
      //printf("\n\rfullKey:%s",fullKey);   
      ret = uciw_get_value(fullKey, value, valueLen);
      #if 0
      if (ret == UCI_OK)
      {
        printf("\n\rvalue:%s",value);     
      }
      #endif
    }
  }


  return ret;
}

int uciw_set_value_by_index(char* key,char* value)
{
  //struct uci_ptr ptr;
  int ret = UCI_OK;
  char package[SMALL_KEY_SIZE]; 
  char section[SMALL_KEY_SIZE];
  char instance[SMALL_KEY_SIZE];
  char option[SMALL_KEY_SIZE]; 
  int sectionEntryPos;
  char fullKey[LARGE_KEY_SIZE];
  
  if ((ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

  /* extract the package name, section name ,option name and instance value from input variable "key" */
  ret = extract_table_token(key, package, section, instance, option);
  //printf("\n\rpackage:%s,section name:%s,option:%s,instance:%s",package,section,option,instance);
  if ( ret == UCI_OK ) /* the multiple instance */
  {
    /* Find the section entry that contains the value of the option "index" to be the same as the variable 'instance' */
    ret = find_table_index(package, section, instance, &sectionEntryPos);
    //printf("\n\rret:%d,sectionEntryPos:%d",ret,sectionEntryPos);    
    if (ret == UCI_OK)
    {
      if (strlen(option)>0)
        sprintf(fullKey,"%s.@%s[%d].%s",package,section,sectionEntryPos,option);
      else
        sprintf(fullKey,"%s.@%s[%d]",package,section,sectionEntryPos);
      //printf("\n\rfullKey:%s",fullKey);   
      ret = uciw_set_value(fullKey, value);
      #if 0
      if (ret == UCI_OK)
      {
        printf("\n\rset value to %s OK",value);     
      }
      #endif
    }
  }
  
  return ret;
}

int uciw_add_section_by_index(char* packageName,char* setionType,char* index)
{
  //struct uci_package *p = NULL;
  //struct uci_section *s = NULL;
  //int ret;
  //struct uci_ptr ptr;
  //char   changedSection[MAX_KEY_SIZE];
  //struct uci_ptr ptrChange;
  //int retTrace;

  //struct uci_ptr ptr;
  int ret = UCI_OK;
  //char package[SMALL_KEY_SIZE]; 
  //char section[SMALL_KEY_SIZE];
  //char instance[SMALL_KEY_SIZE];
  //char option[SMALL_KEY_SIZE]; 
  int sectionEntryPos;
  char fullKey[LARGE_KEY_SIZE];
  
  if ((ctx==NULL) || (packageName==NULL) || (setionType==NULL) || (index==NULL))
  {
    return UCI_ERR_INVAL;
  }

  /* Find the section entry that contains the value of the option "index" to be the same as the variable 'instance' */
  ret = find_table_index(packageName, setionType, index, &sectionEntryPos);
  //printf("\n\rret:%d,sectionEntryPos:%d",ret,sectionEntryPos);    
  if (ret == UCI_ERR_NOTFOUND) /* the section with the index is not exist, add it */
  {
    /* add a new section */
    ret = uciw_add_section(packageName, setionType);
    /* set the index value to be the new section */
    sprintf(fullKey,"%s.@%s[-1].%s",packageName,setionType,UCI_TABLE_INDEX);
    //printf("\n\rfullKey:%s",fullKey);   
    ret= uciw_set_value(fullKey, index);
    #if 0
    if (ret == UCI_OK)
    {
      printf("\n\rset %s value to %s OK",fullKey,index);      
    }
    #endif
  }
  else /* the section entry with the index is exist */
  { 
    ret = UCI_ERR_DUPLICATE;
  }
  
  return ret;
}

int uciw_delete_key_by_index(char* key)
{
  //struct uci_ptr ptr;
  int ret = UCI_OK;
  char package[SMALL_KEY_SIZE]; 
  char section[SMALL_KEY_SIZE];
  char instance[SMALL_KEY_SIZE];
  char option[SMALL_KEY_SIZE]; 
  int sectionEntryPos;
  char fullKey[LARGE_KEY_SIZE];
  
  if ((ctx==NULL) || (key==NULL))
  {
    return UCI_ERR_INVAL;
  }

  /* extract the package name, section name ,option name and instance value from input variable "key" */
  ret = extract_table_token(key, package, section, instance, option);
  //printf("\n\rpackage:%s,section name:%s,option:%s,instance:%s",package,section,option,instance);
  if ( ret == UCI_OK ) /* the multiple instance */
  {
    /* Find the section entry that contains the value of the option "index" to be the same as the variable 'instance' */
    ret = find_table_index(package, section, instance, &sectionEntryPos);
    //printf("\n\rret:%d,sectionEntryPos:%d",ret,sectionEntryPos);    
    if (ret == UCI_OK)
    {
      if (strlen(option)>0)
        sprintf(fullKey,"%s.@%s[%d].%s",package,section,sectionEntryPos,option);
      else
        sprintf(fullKey,"%s.@%s[%d]",package,section,sectionEntryPos);
      //printf("\n\rfullKey:%s",fullKey);   
      ret = uciw_delete_key(fullKey);
      #if 0
      if (ret == UCI_OK)
      {
        printf("\n\rdelete key %s OK",fullKey);     
      }
      #endif
    }
  }
  
  return ret;
}

/* uciw_delete_section() is the same as uciw_delete_key() */
int uciw_delete_section_by_index(char* section)
{
  return uciw_delete_key_by_index(section);
}

int uciw_add_value_by_index(char* key, char* value)
{
  //struct uci_ptr ptr;
  int ret = UCI_OK;
  char package[SMALL_KEY_SIZE]; 
  char section[SMALL_KEY_SIZE];
  char instance[SMALL_KEY_SIZE];
  char option[SMALL_KEY_SIZE]; 
  int sectionEntryPos;
  char fullKey[LARGE_KEY_SIZE];
  
  if ((ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

  /* extract the package name, section name ,option name and instance value from input variable "key" */
  ret = extract_table_token(key, package, section, instance, option);
  //printf("\n\rpackage:%s,section name:%s,option:%s,instance:%s",package,section,option,instance);
  if ( ret == UCI_OK ) /* the multiple instance */
  {
    /* Find the section entry that contains the value of the option "index" to be the same as the variable 'instance' */
    ret = find_table_index(package, section, instance, &sectionEntryPos);
    //printf("\n\rret:%d,sectionEntryPos:%d",ret,sectionEntryPos);    
    if (ret == UCI_OK)
    {
      if (strlen(option)>0)
        sprintf(fullKey,"%s.@%s[%d].%s",package,section,sectionEntryPos,option);
      else
        sprintf(fullKey,"%s.@%s[%d]",package,section,sectionEntryPos);
      //printf("\n\rfullKey:%s",fullKey);   
      ret = uciw_add_value(fullKey, value);
      #if 0
      if (ret == UCI_OK)
      {
        printf("\n\rset value to %s OK",value);     
      }
      #endif
    }
  }
  
  return ret;
}

int uciw_delete_value_by_index(char* key, char* value)
{
  //struct uci_ptr ptr;
  int ret = UCI_OK;
  char package[SMALL_KEY_SIZE]; 
  char section[SMALL_KEY_SIZE];
  char instance[SMALL_KEY_SIZE];
  char option[SMALL_KEY_SIZE]; 
  int sectionEntryPos;
  char fullKey[LARGE_KEY_SIZE];
  
  if ((ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

  /* extract the package name, section name ,option name and instance value from input variable "key" */
  ret = extract_table_token(key, package, section, instance, option);
  //printf("\n\rpackage:%s,section name:%s,option:%s,instance:%s",package,section,option,instance);
  if ( ret == UCI_OK ) /* the multiple instance */
  {
    /* Find the section entry that contains the value of the option "index" to be the same as the variable 'instance' */
    ret = find_table_index(package, section, instance, &sectionEntryPos);
    //printf("\n\rret:%d,sectionEntryPos:%d",ret,sectionEntryPos);    
    if (ret == UCI_OK)
    {
      if (strlen(option)>0)
        sprintf(fullKey,"%s.@%s[%d].%s",package,section,sectionEntryPos,option);
      else
        sprintf(fullKey,"%s.@%s[%d]",package,section,sectionEntryPos);
      //printf("\n\rfullKey:%s",fullKey);   
      ret = uciw_delete_value(fullKey, value);
      #if 0
      if (ret == UCI_OK)
      {
        printf("\n\rset value to %s OK",value);     
      }
      #endif
    }
  }
  
  return ret;
}

int uciw_set_new_value_by_index(char* key,char* sectionType, char* value)
{
  //struct uci_ptr ptr;
  int ret = UCI_OK;
  char package[SMALL_KEY_SIZE]; 
  char section[SMALL_KEY_SIZE];
  char instance[SMALL_KEY_SIZE];
  char option[SMALL_KEY_SIZE]; 
  int sectionEntryPos;
  char fullKey[LARGE_KEY_SIZE];
  struct stat sb;
  char fullName[LARGE_KEY_SIZE];
  int fd;
  
  if ((ctx==NULL) || (key==NULL) || (value==NULL))
  {
    return UCI_ERR_INVAL;
  }

  /* extract the package name, section name ,option name and instance value from input variable "key" */
  ret = extract_table_token(key, package, section, instance, option);
  //printf("\n\rpackage:%s,section name:%s,option:%s,instance:%s",package,section,option,instance);
  if ( ret == UCI_OK ) /* the multiple instance */
  {
    /* Find the section entry that contains the value of the option "index" to be the same as the variable 'instance' */
    ret = find_table_index(package, section, instance, &sectionEntryPos);
    //printf("\n\rret:%d,sectionEntryPos:%d",ret,sectionEntryPos);    
    if (ret == UCI_OK) /* find the section entry */
    {
      if (strlen(option)>0)
        sprintf(fullKey,"%s.@%s[%d].%s",package,section,sectionEntryPos,option);
      else
        sprintf(fullKey,"%s.@%s[%d]",package,section,sectionEntryPos);
      //printf("\n\rfullKey:%s",fullKey);   
      ret = uciw_set_value(fullKey, value);
      #if 0
      if (ret == UCI_OK)
      {
        printf("\n\rset value to %s OK",value);     
      }
      #endif
    }
    else  
    {
      /* check if the file is exist */
      sprintf(fullName,"%s/%s",ctx->confdir,package);
      if (stat(fullName, &sb) == -1)  /* file is not exist, create it */
        {
              fd = open(fullName, O_CREAT);
        if (fd)
          close(fd);
      }

      /* create the section and the option 'index' */
      ret = uciw_add_section_by_index(package, section, instance);
      /* set the value to the new section */
      if (strlen(option)>0)
      {
        sprintf(fullKey,"%s.@%s[-1].%s",package,section,option);
       
        //printf("\n\rfullKey:%s",fullKey);   
        ret = uciw_set_value(fullKey, value);
        #if 0
        if (ret == UCI_OK)
        {
          printf("\n\rset value to %s OK",value);     
        }
        #endif
      }
      
    }
  }
  
  return ret;
}
/* end of jack tseng 03/23/2010 */



