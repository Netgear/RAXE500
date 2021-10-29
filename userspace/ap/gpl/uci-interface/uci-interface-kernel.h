#ifndef __UCI_INTERFACE_H
#define __UCI_INTERFACE_KERNEL_H

//#include "uci.h"

enum
{
	UCI_OK = 0,
	UCI_ERR_MEM,
	UCI_ERR_INVAL,
	UCI_ERR_NOTFOUND,
	UCI_ERR_IO,
	UCI_ERR_PARSE,
	UCI_ERR_DUPLICATE,
	UCI_ERR_UNKNOWN,
	UCI_ERR_LAST
};


#define MAX_MAX_KEY_SIZE	65536
#define MAX_KEY_SIZE		512
#define LARGE_KEY_SIZE		256
#define MEDIUM_KEY_SIZE	128
#define SMALL_KEY_SIZE		64
#define TINY_KEY_SIZE		32

#define UCI_TRACE_SAVE_FAIL UCI_ERR_LAST+1 /* save the package change into file failed */

/* jack tseng 01/14/2010 add for transaction lock */
 #define UCI_LOCK_FILE			"/etc/config/tlock"
 #define UCI_TRANSACTION_FILE 	"/etc/config/transaction"
 /* end of jack tseng 01/14/2010 */


void uciw_dump(char* package);
int uciw_get_value(char* key,char* value,int valueLen);
int uciw_set_value(char* key,char* value);
int uciw_set_new_value(char* key,char* sectionType, char* value); /* jack tseng 03/17/2010 add */
int uciw_delete_key(char* key);
int uciw_delete_section(char* section);
int uciw_commit(char* package);
int uciw_commit_force(char* package); /* jack tseng 11/17/2010 add for reset to default */
int uciw_add_value(char* key, char* value);
int uciw_delete_value(char* key, char* value);
int uciw_set_config_dir(char* path);
#if 1
int uciw_get_changes(char* package, char* value,int valueLen);
#else
char* uci_get_changes(char* package);
#endif
int uciw_update(char* packageName);
int uciw_import_file(char* importFilename, char* packageName);
int uciw_merge_file(char* directory, char* file, char* package); /* jack tseng 09/06/2010 merge the file to the package */
int uciw_export_file(char* exportFilename, char* packageName);
int uciw_add_section(char* packageName,char* setionType); /* jack tseng 12/08/2009 add */
int uciw_init(void);
int uciw_exit(void);
int uciw_get_sequence(char* sequence,int len); /* jack tseng 03/16/2010 add */

/* jack tseng 03/22/2010 add for the sets of the index functions */
int uciw_get_value_by_index(char* key,char* value,int valueLen); 
int uciw_set_value_by_index(char* key,char* value); 
int uciw_add_section_by_index(char* packageName,char* setionType,char* index);
int uciw_delete_key_by_index(char* key);
int uciw_delete_section_by_index(char* section);
int uciw_add_value_by_index(char* key, char* value);
int uciw_delete_value_by_index(char* key, char* value);
int uciw_set_new_value_by_index(char* key,char* sectionType, char* value);
#if 0 /* jack tseng 02/04/2010 remove */
/* jack tseng 01/19/2010 add for transaction */
int uci_commit_transaction(char* package, char* transID);
int uci_get_transaction_result(char* transID,char* result, int resultLen);
int uci_set_transaction_result(char* transID,char* package,char* result);
#endif
#endif
