/* util.c ....... error message utilities.
 *                C. Scott Ananian <cananian@alumni.princeton.edu>
 *
 * $Id: util.c,v 1.10 2005/03/10 01:18:20 quozl Exp $
 */

#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <stdlib.h>
#include "util.h"

#ifdef CODE_IN_USE  //Winster Chan added 05/16/2006
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "pptp"
#endif
#endif  //CODE_IN_USE Winster Chan added 05/16/2006

/* implementation of log_string, defined as extern in util.h */
char *log_string = "anon";

#ifdef CODE_IN_USE  //Winster Chan added 05/16/2006
static void open_log(void) __attribute__ ((constructor));
static void close_log(void) __attribute__ ((destructor));
#endif  //CODE_IN_USE Winster Chan added 05/16/2006

#define MAKE_STRING(label) 				\
va_list ap;						\
char buf[256], string[256];				\
va_start(ap, format);					\
vsnprintf(buf, sizeof(buf), format, ap);		\
snprintf(string, sizeof(string), "%s %s[%s:%s:%d]: %s",	\
	 log_string, label, func, file, line, buf);	\
va_end(ap)

#ifdef CODE_IN_USE  //Winster Chan added 05/16/2006
/*** open log *****************************************************************/
static void open_log(void) {
    openlog(PROGRAM_NAME, LOG_PID, LOG_DAEMON);
}

/*** close log ****************************************************************/
static void close_log(void)
{
    closelog();
}
#endif  //CODE_IN_USE Winster Chan added 05/16/2006
#ifdef nostrip //Modified by Silver to shrink code
/*** print a message to syslog ************************************************/
void _log(const char *func, const char *file, int line, const char *format, ...)
{
    MAKE_STRING("log");
#if defined(USE_SYSLOG) /* foxconn wklin added, 08/13/2007 */
    syslog(LOG_NOTICE, "%s", string);
#endif
}

/*** print a warning to syslog ************************************************/
void _warn(const char *func, const char *file, int line, const char *format, ...)
{
    MAKE_STRING("warn");
    fprintf(stderr, "%s\n", string);
#if defined(USE_SYSLOG) /* foxconn wklin added, 08/13/2007 */
    syslog(LOG_WARNING, "%s", string);
#endif
}

/*** print a fatal warning to syslog and exit *********************************/
void _fatal(const char *func, const char *file, int line, const char *format, ...)
{
    MAKE_STRING("fatal");
    fprintf(stderr, "%s\n", string);
#if defined(USE_SYSLOG) /* foxconn wklin added, 08/13/2007 */
    syslog(LOG_CRIT, "%s", string);
#endif
    exit(1);
}
#endif
/*** connect a file to a file descriptor **************************************/
int file2fd(const char *path, const char *mode, int fd)
{
    int ok = 0;
    FILE *file = NULL;
    file = fopen(path, mode);
    if (file != NULL && dup2(fileno(file), fd) != -1)
        ok = 1;
    if (file) fclose(file);
    return ok;
}

/* signal to pipe delivery implementation */
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

/* pipe private to process */
static int sigpipe[2];

/* create a signal pipe, returns 0 for success, -1 with errno for failure */
int sigpipe_create()
{
  int rc;
  
  rc = pipe(sigpipe);
  if (rc < 0) return rc;
  
  fcntl(sigpipe[0], F_SETFD, FD_CLOEXEC);
  fcntl(sigpipe[1], F_SETFD, FD_CLOEXEC);
  
#ifdef O_NONBLOCK
#define FLAG_TO_SET O_NONBLOCK
#else
#ifdef SYSV
#define FLAG_TO_SET O_NDELAY
#else /* BSD */
#define FLAG_TO_SET FNDELAY
#endif
#endif
  
  rc = fcntl(sigpipe[1], F_GETFL);
  if (rc != -1)
    rc = fcntl(sigpipe[1], F_SETFL, rc | FLAG_TO_SET);
  if (rc < 0) return rc;
  return 0;
#undef FLAG_TO_SET
}

/* generic handler for signals, writes signal number to pipe */
void sigpipe_handler(int signum)
{
  write(sigpipe[1], &signum, sizeof(signum));
  signal(signum, sigpipe_handler);
}

/* assign a signal number to the pipe */
void sigpipe_assign(int signum)
{
  sigset_t sigset;
  struct sigaction sa;

  sigemptyset(&sigset);
  sigaddset(&sigset, signum);
  
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = sigpipe_handler;
  sigaction(signum, &sa, NULL);
}

/* return the signal pipe read file descriptor for select(2) */
int sigpipe_fd()
{
  return sigpipe[0];
}

/* read and return the pending signal from the pipe */
int sigpipe_read()
{
  int signum;
  read(sigpipe[0], &signum, sizeof(signum));
  return signum;
}

void sigpipe_close()
{
  close(sigpipe[0]);
  close(sigpipe[1]);
}

