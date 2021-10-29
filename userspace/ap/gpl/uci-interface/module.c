#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h> /* jack tseng 12/17/2009 add for lock/unlock file */
#include <errno.h>
#include "uci-interface.h"

#define MAX_ARGS  4 /* max command line arguments for batch mode */

#if 0 /****** for test ********/
#define CONFIG_DIR  "/home/jack/wimax/uci/uci.git/config"
#define CONFIG1_DIR "/home/jack/wimax/uci/uci.git/config1"
#else /******** for F1 board ********/
#define CONFIG_DIR  "/etc/config"
#endif

//static const char *delimiter = " ";
static const char *appname;
static enum {
  CLI_FLAG_MERGE =    (1 << 0),
  CLI_FLAG_QUIET =    (1 << 1),
  CLI_FLAG_NOCOMMIT = (1 << 2),
  CLI_FLAG_BATCH =    (1 << 3),
  CLI_FLAG_SHOW_EXT = (1 << 4),
} flags;

//static FILE *input;

//static struct uci_context *ctx;
enum {
  /* section cmds */
  CMD_GET,
  CMD_SET,
  CMD_ADD_LIST,
  CMD_DEL,
  CMD_RENAME,
  CMD_REVERT,
  /* package cmds */
  CMD_SHOW,
  CMD_CHANGES,
  CMD_EXPORT,
  CMD_COMMIT,
  /* other cmds */
  CMD_ADD,
  CMD_IMPORT,
  CMD_HELP,
};


static void uci_usage(void)
{
  fprintf(stderr,
    "\n\rUsage: %s <command> [<arguments>]\n\r\n\r"
    "Commands:\n\r"
    "\texit\n\r"
    "\tupdate     <config>\n\r"
    "\texport     <filename> [<config>]\n\r"
    "\timport     <filename> [<config>]\n\r"
    "\tmerge     <file path> <filename> [<config>]\n\r"
    "\trec_changes    <config>\n\r"
    "\tcommit     <config>\n\r"
  #if 0 
    "\tcommit_trans     <config>\n\r"
    "\tget_trans_result \n\r"
    "\tset_trans_result <config> <result value>\n\r"
  #endif  
    "\tadd_section        <config> <section-type>\n\r" 
    "\tadd_list   <config>.<section>.<option> <value>\n\r"
    "\tdelete_value   <config>.<section>.<option> <value>\n\r"
    "\tdump       [<config>]\n\r"
    "\tget        <config>.<section>[.<option>]\n\r"
    "\tset        <config>.<section>[.<option>] <value>\n\r"
    "\tset_new        <config>.<section>[.<option>] <value> [<section type>]\n\r"
    "\tdelete     <config>.<section>.<option>\n\r"
    "\tdelete_section     <config>.<section>\n\r"
    "\tchdir     <directory>\n\r"
    "\tstressMulti     <config>[.<section[.<option>]] <repeat times>\n\r"
    "\tstressSet     <config>[.<section[.<option>]] <repeat times>\n\r"
    "\tlock       [<config>]\n\r"
    "\tunlock       [<config>]\n\r"
    "\ttrigger       [<config>]\n\r"
    "\tget_seq       \n\r"
    "\tget_index        <config>.<section>[.<option>]\n\r"
    "\tset_index        <config>.<section>[.<option>] <value>\n\r"
    "\tset_new_index        <config>.<section>[.<option>] <value> [<section type>]\n\r"
    "\tadd_section_index        <config> <section-type> <index>\n\r" 
    "\tdelete_index     <config>.<section>.<option>\n\r"
    "\tdelete_section_index     <config>.<section>\n\r"
    "\tadd_list_index   <config>.<section>.<option> <value>\n\r"
    "\tdelete_value_index   <config>.<section>.<option> <value>\n\r"
    //"\trename     <config>.<section>[.<option>]=<name>\n\r"
    //"\trevert     <config>[.<section>[.<option>]]\n\r"
    "\n\r",
    appname
  );
}

#if 0
static void cli_perror(void)
{
  if (flags & CLI_FLAG_QUIET)
    return;

  uci_perror(ctx, appname);
}
#endif

int main(int argc, char **argv)
{
  int ret;
  //int c;
  //int cmd;
  char value[MAX_KEY_SIZE];
  //char* changes;
  char *find, *command, *params[10];
  char input[1024];
  int paramCount;
  int ii,jj;
  int index,count;
  char testValue[32];
  char testParam1[MAX_KEY_SIZE],testParam2[MAX_KEY_SIZE];
  int fd=0; /* jack tseng 12/17/2009 add for lock/unlock file */
  char fullFileName[256]; /* jack tseng 12/17/2009 add for lock/unlock file */
#ifdef USE_NFS  
  struct flock lockArg; /* jack tseng 12/17/2009 add for lock/unlock file */
#endif

  flags = CLI_FLAG_SHOW_EXT;
  appname = argv[0];
  //input = stdin;

#if 0
  if (argc < 2) 
  {
    uci_usage();
    return 0;
  }
#endif  

  /* initialize UCI library */
  ret = uciw_init();
  if (ret != UCI_OK) 
  {
    fprintf(stderr, "Out of memory\n\r");
    goto done;
  }

  /* set the config directory */
  //uciw_set_config_dir(CONFIG_DIR);
  //uci_set_config_dir(CONFIG1_DIR);

  uci_usage();
  while(fgets(input, sizeof(input), stdin) != NULL)
  {
      if(input[0] == '\n')
    {
      uci_usage();
      continue;
      }
    printf("Your string =====>");
    find = strchr(input, '\n');
    if(find != NULL)
      *find = '\0';

    printf("%s\n\r", input);

    //Get command and parameters
    command = strtok(input, " ");
    printf("Command::%s\n\r", command);

    paramCount=0;
    ii = 0;
    do
    {
      params[ii]= strtok(NULL, " ");
      ii++;
    }while(params[ii-1] != NULL);

    printf("You input %d parameters\n\r", ii-1);
    paramCount = ii-1;

    for(jj=0;jj<ii-1;jj++)
      printf("%dth Parameter::%s\n\r",jj+1, params[jj]);

    if(!strcmp("exit", command) || !strcmp("0", command) )
    {
      break;
    }
      
  
    if (!strcasecmp(command, "dump"))
    {
      if (paramCount==0)
      {
        uciw_dump(NULL);
      }   
      else if (paramCount == 1)
      {
        uciw_dump(params[0]);
      }
      else
      {
        uci_usage();
        //goto done;    
      }
    }
    else if (!strcasecmp(command, "commit"))
    {
      if (paramCount==0)
      {
        printf("\n\r[%s,%d]",__FUNCTION__,__LINE__);
        uciw_commit(NULL);
        if (ret == UCI_OK)
          printf("\ncommit all pakage ok\n\r");
      }   
      else if (paramCount == 1)
      {
        ret = uciw_commit(params[0]);
        if (ret == UCI_OK)
          printf("\ncommit pakage %s ok\n\r",params[0]);
      }   
      else
      {
        uci_usage();
      }
    }
    #if 0
    else if (!strcasecmp(command, "commit_trans"))
    {
      if (paramCount==0)
      {
        printf("\n\r[%s,%d]",__FUNCTION__,__LINE__);
        uci_commit_transaction(NULL,"web001");
        if (ret == UCI_OK)
          printf("\ncommit all pakage of the transaction ok\n\r");
      }   
      else if (paramCount == 1)
      {
        ret = uci_commit_transaction(params[0],"web001");
        if (ret == UCI_OK)
          printf("\ncommit pakage %s of the transaction ok\n\r",params[0]);
      }   
      else
      {
        uci_usage();
      }
    }
    else if (!strcasecmp(command, "get_trans_result"))
    {
      if (paramCount==0)
      {
        printf("\n\r[%s,%d]",__FUNCTION__,__LINE__);
        uci_get_transaction_result("web001",value,MAX_KEY_SIZE);
        if (ret == UCI_OK)
          printf("\n The transaction(id:web001) result: %s\n\r",value);
      }   
      else
      {
        uci_usage();
      }
    }
    else if (!strcasecmp(command, "set_trans_result"))
    {
      if (paramCount != 2)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uci_set_transaction_result("web001",params[0], params[1]);
        if (ret == UCI_OK)
          printf("\n\rset trans ok!! id: web001, package:%s, result:%s\n\r",params[0],params[1]);
      }
    }
    #endif
    else if (!strcasecmp(command, "get"))
    {
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_get_value(params[0], value, MAX_KEY_SIZE);
        //ret = uci_get_value(params[0], value, 4);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s\n\r",params[0],value);
      }
    }
    else if (!strcasecmp(command, "set"))
    {
      if (paramCount != 2)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_set_value(params[0], params[1]);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s\n\r",params[0],params[1]);
      }
    }
    else if (!strcasecmp(command, "set_new"))
    {
      if (paramCount == 2)
      {
        ret = uciw_set_new_value(params[0], NULL, params[1]);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s\n\r",params[0],params[1]);
      }
      else if (paramCount == 3)
      {
        ret = uciw_set_new_value(params[0], params[2], params[1]);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s,section-type:%s\n\r",params[0],params[1],params[2]);
      }
      else
      {
        uci_usage();
      }
    }
//    else if (!strcasecmp(command, "ren") ||
//             !strcasecmp(command, "rename"))
//      cmd = CMD_RENAME;
    else if (!strcasecmp(command, "update")) /* reload the config file */
    {
      /* jack tseng 12/29/2009 modification */
      /* allow 0 parameters. If the number of parameters is 0, update all packages */
      if (paramCount==0)
      {
        ret = uciw_update(NULL);
        if (ret == UCI_OK)
          printf("\n\rAll packages are update ok\n\r");
      }   
      else if (paramCount == 1)
      {
        ret = uciw_update(params[0]);
        if (ret == UCI_OK)
          printf("\n\rPackage %s is update ok\n\r",params[0]);
      }
      else
      {
        uci_usage();
      }
    }
    else if (!strcasecmp(command, "rec_changes")) /* get the list of the changed keys */
    {
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        //changes = uci_get_changes(params[0]);
        ret = uciw_get_changes(params[0],value, MAX_KEY_SIZE);
        //ret = uci_get_changes(params[0],value, 10);
        if (ret == UCI_OK)
        {
          printf("\n\rchanges:%s\n\r",value);
          //free(changes);
        }
      }
    }
    else if (!strcasecmp(command, "del") ||
             !strcasecmp(command, "delete"))
    {
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_delete_key(params[0]);
        if (ret == UCI_OK)
          printf("\n\rkey %s is deleted ok\n\r",params[0]);
      }
    } 
    else if (!strcasecmp(command, "del_sec") ||
             !strcasecmp(command, "delete_section"))
    {
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_delete_section(params[0]);
        if (ret == UCI_OK)
          printf("\n\rsection %s is deleted ok\n\r",params[0]);
      }
    }
    else if (!strcasecmp(command, "import"))
    {
      //uci_import_file
      if (paramCount == 1)
      {
        ret = uciw_import_file(params[0], NULL);
        if (ret == UCI_OK)
          printf("\n\rfile %s is imported ok\n\r",params[0]);
      }   
      else if (paramCount == 2)
      {
        ret = uciw_import_file(params[0],params[1]);
        if (ret == UCI_OK)
        printf("\n\rfile %s is merged to %s ok\n\r",params[0],params[1]);
      }
      else
      {
        uci_usage();
        //goto done;    
      } 
    }
    else if (!strcasecmp(command, "merge"))
    {
      //uci_import_file
      if (paramCount == 2)
      {
        ret = uciw_merge_file(params[0], params[1],params[1]);
        if (ret == UCI_OK)
          printf("\n\rfile %s %s is imported ok\n\r",params[0],params[1]);
      }   
      else if (paramCount == 3)
      {
        ret = uciw_merge_file(params[0],params[1],params[2]);
        if (ret == UCI_OK)
        printf("\n\rfile %s is merged to %s ok\n\r", params[1], params[2]);
      }
      else
      {
        uci_usage();
        //goto done;    
      } 
    }
    else if (!strcasecmp(command, "export"))
    {
      //uci_export_file
      if (paramCount == 1)
      {
        ret = uciw_export_file(params[0], NULL);
        if (ret == UCI_OK)
          printf("\n\rall packages are exported to %s ok\n\r",params[0]);
      }   
      else if (paramCount == 2)
      {
        ret = uciw_export_file(params[0],params[1]);
        if (ret == UCI_OK)
          printf("\n\rpackage %s is exported to %s ok\n\r",params[1],params[0]);
      }
      else
      {
        uci_usage();
        //goto done;    
      } 
    }
//    else if (!strcasecmp(command, "help"))
//      cmd = CMD_HELP;
    else if (!strcasecmp(command, "add_list"))
    {
      if (paramCount != 2)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_add_value(params[0], params[1]);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s\n\r",params[0],params[1]);
      }
    }
    else if (!strcasecmp(command, "delete_value"))
    {
      if (paramCount != 2)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_delete_value(params[0], params[1]);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s\n\r",params[0],params[1]);
      }
    }
  /* jack tseng 12/08/2009 add */
  /* "add" command can add a new section type without section name */
    else if (!strcasecmp(command, "add_section"))
    {
      //printf("\n\r [%s,%d]paramCount:%d \n\r",__FUNCTION__,__LINE__,paramCount);
      if (paramCount != 2)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        printf("\n\r uci add section\n\r");
        ret = uciw_add_section(params[0], params[1]);
        if (ret == UCI_OK)
          printf("\n\rpackage:%s,section-type:%s\n\r",params[0],params[1]);
      }
    }
  /* end of jack tseng 12/08/2009 */  
    else if (!strcasecmp(command, "chdir")) /* change the working directory */
    {
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_set_config_dir(params[0]);
        if (ret==UCI_OK)
        {
          printf("\n\rchange working directory %s ok\n\r",params[0]);
        }
      }
    }
    else if (!strcasecmp(command, "stressMulti"))
    {
      if (paramCount == 2)
      {
        strcpy(testParam1, params[0]);
        count = atoi(params[1]);
        for (index=0;index<count;index++)
        {
          printf("\n\r********%d th *********",index);
          
          /* get */
          strcpy(testParam2, testParam1);
          ret = uciw_get_value(testParam2, value, MAX_KEY_SIZE);
          if (ret == UCI_OK)
          {
            printf("\n\rkey:%s,value:%s\n\r",testParam1,value);
            fflush(stdout);
          }
          
          #if 0
          /* set */
          sprintf(testValue, "%d", index);
          strcpy(testParam2, testParam1);
          ret = uci_set_value(testParam2, testValue);
          if (ret == UCI_OK)
            printf("\n\rkey:%s,value:%s\n\r",testParam1,testValue); 
          /* commit */
          ret = uci_commit_package("import_temp");
          if (ret == UCI_OK)
            printf("\n\rcommit pakage %s ok\n\r","import_temp");
          #endif
          
          /* update */
          #if 1
          //ret = uci_update("import_temp");
          ret = uciw_update(NULL);
          if (ret == UCI_OK)
          {
            printf("\n\rPackage %s is update ok\n\r",testParam2);
            fflush(stdout);
          }
          #endif

          if ((index>0) && (index%1000==0))
            sleep(5);
        }
      }
      else
      {
        uci_usage();
        //goto done;    
      } 
    }
    else if (!strcasecmp(command, "stressSet"))
    {
      if (paramCount == 2)
      {
        count = atoi(params[1]);
        for (index=0;index<count;index++)
        {
          
          /* set */
          sprintf(testValue, "%d", index*2);
          strcpy(testParam1, params[0]);
          printf("\n\r********%d th,key:%s *********",index,testParam1);
          ret = uciw_set_value(testParam1, testValue);
          if (ret == UCI_OK)
            printf("\n\rkey:%s,value:%s\n\r",params[0],testValue);  
          
          /* commit */
          ret = uciw_commit("Dhcpv4Server");
          if (ret == UCI_OK)
            printf("\n\rcommit Dhcpv4Server pakage ok\n\r");
        #if 1 
          if ((index>0) && (index%1000==0))
            sleep(5);
        #else 
          /* wait for inotiy-agent action */
          sleep(5);
        #endif
        }
      }
      else
      {
        uci_usage();
        //goto done;    
      } 
    }
  /* jack tseng 12/17/2009 add for lock/unlock file */  
    else if (!strcasecmp(command, "lock"))
    {
    /* int mode = (write ? O_RDWR : O_RDONLY); */
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        sprintf(fullFileName,"/etc/config/%s",params[0]);
        
        /* lock the config file */
        fd = open(fullFileName, O_RDWR|O_EXCL, 0600);
        if (fd < 0)
          printf("\nopen file %s fail\n\r",fullFileName);
        
      #if USE_NFS
        memset(&lockArg, 0, sizeof(struct flock));
        lockArg.l_type = F_WRLCK;
        lockArg.l_len= 0;
        lockArg.l_whence = SEEK_SET;
        lockArg.l_start = 0;

    /* 
      If using fcntl(), the same process can lock the same file successfully for multiple times.
      If using fcntl(), the different process can't lock the same file.
      If using flock(), the same process can't lock the same file for multiple times.
      If using flock(), the different process can't lock the same file.
      
    */
        //ret = fcntl(fd, F_SETLK, &lockArg);   
        ret = fcntl(fd, F_SETLKW, &lockArg);    
      #else
        ret = flock(fd, LOCK_EX|LOCK_NB);
      #endif  
        if ((ret < 0) && (errno != ENOSYS))
          printf("\nlock pakage %s fail\n\r",params[0]);
        else
          printf("\nlock pakage %s ok\n\r",params[0]);
      
      }
    }
    else if (!strcasecmp(command, "unlock"))
    {
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        if (fd > 0)
        {
        #if USE_NFS
          memset(&lockArg, 0, sizeof(struct flock));
          lockArg.l_type = F_UNLCK;
          lockArg.l_len= 0;
          lockArg.l_whence = SEEK_SET;
          lockArg.l_start = 0;

          ret = fcntl(fd, F_SETLK, &lockArg);   
        #else
          ret = flock(fd, LOCK_UN);
        #endif
          if ((ret < 0) && (errno != ENOSYS))
            printf("\nunlock pakage %s fail\n\r",params[0]);
          else
            printf("\nunlock pakage %s ok\n\r",params[0]);
          close(fd);
          fd = 0;
        }
      }
    }
    else if (!strcasecmp(command, "trigger"))
    {
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        if (strstr(params[0],"if")) 
          sprintf(fullFileName,"/tmp/var/cpe/trigger/inotify_agt/%s",params[0]);
        else
          sprintf(fullFileName,"/tmp/var/cpe/trigger/share/global/%s",params[0]);

        printf("\nopen file %s\n\r",fullFileName);
        /* open the file */
        fd = open(fullFileName, O_RDWR);
        //fd = open(fullFileName, O_RDONLY);
        if (fd < 0)
          printf("\nopen file %s fail\n\r",fullFileName);
        else
          close(fd);
      
      }
    }
  /* end of jack tseng 12/17/2009 */  
    else if (!strcasecmp(command, "get_seq"))
    {
      if (paramCount != 0)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        //for (ii=0;ii<5;ii++)
        {
          ret = uciw_get_sequence(testValue,32);
          if (ret == UCI_OK)
            printf("\nget sequence %s ok\n\r",testValue);
        }
      
      }
    }
  /* end of jack tseng 12/17/2009 */  
    else if (!strcasecmp(command, "get_index"))
    {
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_get_value_by_index(params[0], value, MAX_KEY_SIZE);
        //ret = uci_get_value(params[0], value, 4);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s\n\r",params[0],value);
      }
    }
    else if (!strcasecmp(command, "set_index"))
    {
      if (paramCount != 2)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_set_value_by_index(params[0], params[1]);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s\n\r",params[0],params[1]);
      }
    }
    else if (!strcasecmp(command, "set_new_index"))
    {
      if (paramCount == 2)
      {
        ret = uciw_set_new_value_by_index(params[0], NULL, params[1]);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s\n\r",params[0],params[1]);
      }
      else if (paramCount == 3)
      {
        ret = uciw_set_new_value_by_index(params[0], params[2], params[1]);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s,section-type:%s\n\r",params[0],params[1],params[2]);
      }
      else
      {
        uci_usage();
      }
    }
    else if (!strcasecmp(command, "add_section_index"))
    {
      //printf("\n\r [%s,%d]paramCount:%d \n\r",__FUNCTION__,__LINE__,paramCount);
      if (paramCount != 3)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        printf("\n\r uci add section by index\n\r");
        ret = uciw_add_section_by_index(params[0], params[1],params[2]);
        if (ret == UCI_OK)
          printf("\n\rpackage:%s,section-type:%s,index:%s\n\r",params[0],params[1],params[2]);
      }
    }
    else if (!strcasecmp(command, "delete_index"))
    {
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_delete_key_by_index(params[0]);
        if (ret == UCI_OK)
          printf("\n\rsection %s is deleted ok\n\r",params[0]);
      }
    }
    else if (!strcasecmp(command, "delete_section_index"))
    {
      if (paramCount != 1)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_delete_section_by_index(params[0]);
        if (ret == UCI_OK)
          printf("\n\rsection %s is deleted ok\n\r",params[0]);
      }
    }
    else if (!strcasecmp(command, "add_list_index"))
    {
      if (paramCount != 2)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_add_value_by_index(params[0], params[1]);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s\n\r",params[0],params[1]);
      }
    }
    else if (!strcasecmp(command, "delete_value_index"))
    {
      if (paramCount != 2)
      {
        uci_usage();
        //goto done;    
      }   
      else
      {
        ret = uciw_delete_value_by_index(params[0], params[1]);
        if (ret == UCI_OK)
          printf("\n\rkey:%s,value:%s\n\r",params[0],params[1]);
      }
    }
    else
    {
//      cmd = -1;
      printf("@@@@@@@@@@ Unknown Command Type @@@@@@@@@@\n\r");
    }
    uci_usage();
  }
done: 
  ret = uciw_exit();


  return ret;
}
