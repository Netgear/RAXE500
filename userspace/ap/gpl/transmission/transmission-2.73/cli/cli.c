/******************************************************************************
 * $Id: cli.c 13473 2012-09-07 02:45:49Z livings124 $
 *
 * Copyright (c) Transmission authors and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************************/

#include <stdio.h> /* fprintf() */
#include <stdlib.h> /* atoi() */
#include <string.h> /* memcmp() */
#include <signal.h>

#include <libtransmission/transmission.h>
#include <libtransmission/bencode.h>
#include <libtransmission/tr-getopt.h>
#include <libtransmission/utils.h> /* tr_wait_msec */
#include <libtransmission/version.h>
#include <libtransmission/web.h> /* tr_webRun */

/***
****
***/

#define MEM_K 1024
#define MEM_K_STR "KiB"
#define MEM_M_STR "MiB"
#define MEM_G_STR "GiB"
#define MEM_T_STR "TiB"

#define DISK_K 1000
#define DISK_B_STR   "B"
#define DISK_K_STR "kB"
#define DISK_M_STR "MB"
#define DISK_G_STR "GB"
#define DISK_T_STR "TB"

#define SPEED_K 1000
#define SPEED_B_STR  "B/s"
#define SPEED_K_STR "kB/s"
#define SPEED_M_STR "MB/s"
#define SPEED_G_STR "GB/s"
#define SPEED_T_STR "TB/s"

/***
****
***/

#define LINEWIDTH 80
#define MY_CONFIG_NAME "transmission"
#define MY_READABLE_NAME "transmission-cli"

static bool showVersion              = false;
static bool showInfo                 = 0; /* Foxconn add,edward zhang, 2012/11/23 @add the option Show torrent details */
static bool verify                   = 0;
static sig_atomic_t gotsig           = 0;
static sig_atomic_t manualUpdate     = 0;

#define	PID_FILE			"/var/run/transmission.pid"
#define MAX_STAU_PATH  1024
char ram_status_file[MAX_STAU_PATH] = {0};
static int stop_process = 0;
static const char * torrentPath  = NULL;

static const struct tr_option options[] =
{
    { 'b', "blocklist",            "Enable peer blocklists", "b",  0, NULL },
    { 'B', "no-blocklist",         "Disable peer blocklists", "B",  0, NULL },
    { 'd', "downlimit",            "Set max download speed in "SPEED_K_STR, "d",  1, "<speed>" },
    { 'D', "no-downlimit",         "Don't limit the download speed", "D",  0, NULL },
    { 910, "encryption-required",  "Encrypt all peer connections", "er", 0, NULL },
    { 911, "encryption-preferred", "Prefer encrypted peer connections", "ep", 0, NULL },
    { 912, "encryption-tolerated", "Prefer unencrypted peer connections", "et", 0, NULL },
    { 'f', "finish",               "Run a script when the torrent finishes", "f", 1, "<script>" },
    { 'g', "config-dir",           "Where to find configuration files", "g", 1, "<path>" },
    { 'i', "info",                 "Show torrent details and exit", "i",  0, NULL        },
    { 'm', "portmap",              "Enable portmapping via NAT-PMP or UPnP", "m",  0, NULL },
    { 'M', "no-portmap",           "Disable portmapping", "M",  0, NULL },
    { 'p', "port", "Port for incoming peers (Default: " TR_DEFAULT_PEER_PORT_STR ")", "p", 1, "<port>" },
    { 't', "tos", "Peer socket TOS (0 to 255, default=" TR_DEFAULT_PEER_SOCKET_TOS_STR ")", "t", 1, "<tos>" },
    { 'u', "uplimit",              "Set max upload speed in "SPEED_K_STR, "u",  1, "<speed>"   },
    { 'U', "no-uplimit",           "Don't limit the upload speed", "U",  0, NULL        },
    { 'v', "verify",               "Verify the specified torrent", "v",  0, NULL        },
    { 'V', "version",              "Show version number and exit", "V", 0, NULL },
    { 'w', "download-dir",         "Where to save downloaded data", "w",  1, "<path>"    },
    { 0, NULL, NULL, NULL, 0, NULL }
};

static const char *
getUsage( void )
{
    return "A fast and easy BitTorrent client\n"
           "\n"
           "Usage: " MY_READABLE_NAME " [options] <file|url|magnet>";
}

static int parseCommandLine( tr_benc*, int argc, const char ** argv );

static void         sigHandler( int signal );

static char*
tr_strlratio( char * buf,
              double ratio,
              size_t buflen )
{
    if( (int)ratio == TR_RATIO_NA )
        tr_strlcpy( buf, _( "None" ), buflen );
    else if( (int)ratio == TR_RATIO_INF )
        tr_strlcpy( buf, "Inf", buflen );
    else if( ratio < 10.0 )
        tr_snprintf( buf, buflen, "%.2f", ratio );
    else if( ratio < 100.0 )
        tr_snprintf( buf, buflen, "%.1f", ratio );
    else
        tr_snprintf( buf, buflen, "%.0f", ratio );
    return buf;
}

static bool waitingOnWeb;

static void
onTorrentFileDownloaded( tr_session   * session UNUSED,
                         bool           did_connect UNUSED,
                         bool           did_timeout UNUSED,
                         long           response_code UNUSED,
                         const void   * response,
                         size_t         response_byte_count,
                         void         * ctor )
{
    tr_ctorSetMetainfo( ctor, response, response_byte_count );
    waitingOnWeb = false;
}

void update_status_file(const tr_stat * st, const tr_torrent  * tor)
{
    FILE *fp=NULL;
    /* update random status file */
    char upStr[80];
    char dnStr[80];
    //tr_formatter_speed_KBps( upStr, st->pieceUploadSpeed_KBps, sizeof( upStr ) );
    //tr_formatter_speed_KBps( dnStr, st->pieceDownloadSpeed_KBps, sizeof( dnStr ) );
    fp=fopen(ram_status_file, "wt");
	if(fp)
	{
    char percent[20];    
		fprintf(fp,"process_pid=%d\n", getpid());
		fprintf(fp,"peer_number=%d\n", st->peersConnected);
		fprintf(fp,"file_name=%s\n", tr_torrentName(tor));
		fprintf(fp,"file_size=%llu\n", st->sizeWhenDone); /*foxconn modified, edward zhang, 2012/12/06 use unsign long long type */

    memset(percent,0,sizeof(percent));
    snprintf(percent,sizeof(percent)-1,"%.1f",tr_truncd( 100 * st->percentDone, 1 ));
    if(tr_truncd( 100 * st->percentDone, 1 )<=100 && (strlen(percent)<=6))
		    fprintf(fp,"finish_percent=%s\n", percent);
		else
		    fprintf(fp,"finish_percent= 0.0\n");
		fprintf(fp,"down_speed=%.0f KB/s\n", st->pieceDownloadSpeed_KBps);
		fprintf(fp,"up_speed=%.0f KB/s\n", st->pieceUploadSpeed_KBps);
		if(st->error == TR_STAT_TRACKER_ERROR)
		    fprintf(fp,"result=Tracker gave an error: %s\n", st->errorString);
		else if(st->error == TR_STAT_TRACKER_ERROR)
		    fprintf(fp,"result=Error: %s\n", st->errorString);
		/* status need update*/
		if( st->activity == TR_STATUS_DOWNLOAD )
	        fprintf(fp,"task_status=%s\n","4");
	    else if(st->activity == TR_STATUS_STOPPED)
	    {
	        /* Download manage restart, stop first, after restart, it still need
	         * auto run task
	         */
	        if(stop_process == 1)
                fprintf(fp,"task_status=%s\n","0");
            else
	            fprintf(fp,"task_status=%s\n","1");
	    }
        else if( st->activity == TR_STATUS_SEED )
	    {
                fprintf(fp,"task_status=%s\n","3");
	    }
        else if(stop_process == 2)
	    {
                fprintf(fp,"task_status=%s\n","1");
	    }
		fflush(fp);
		fclose(fp);
	}
}
/* Foxconn add start,edward zhang, 2012/11/23 @add the option Show torrent details */
static void
dumpInfo( const tr_info * inf )
{
    FILE *out = NULL;
    int             i;
    int             prevTier = -1;
    tr_file_index_t ff;

    out = fopen("/tmp/torrent.info","wt");
	if(!out)
		return ;

    fprintf( out, "hash:\t" );
    for( i = 0; i < SHA_DIGEST_LENGTH; ++i )
        fprintf( out, "%02x", inf->hash[i] );
    fprintf( out, "\n" );

    fprintf( out, "name:\t%s\n", inf->name );

    fprintf( out, "size:\t%" PRIu64 "\n", inf->totalSize);

    if( inf->comment && *inf->comment )
        fprintf( out, "comment:\t%s\n", inf->comment );
    if( inf->creator && *inf->creator )
        fprintf( out, "creator:\t%s\n", inf->creator );
    if( inf->isPrivate )
        fprintf( out, "private flag set\n" );

    fprintf( out, "file(s):\n" );
    for( ff = 0; ff < inf->fileCount; ++ff )
        fprintf( out, "\t%s (%" PRIu64 ")\n", inf->files[ff].name,
                 inf->files[ff].length );
    fflush(out);
	fclose(out);
}
/* Foxconn add end,edward zhang, 2012/11/23 @add the option Show torrent details */

static void
getStatusStr( const tr_stat * st,
              char *          buf,
              size_t          buflen,
              tr_torrent      * tor )
{
    if( st->activity == TR_STATUS_CHECK_WAIT )
    {
        tr_snprintf( buf, buflen, "Waiting to verify local files" );
    }
    else if( st->activity == TR_STATUS_CHECK )
    {
        tr_snprintf( buf, buflen,
                     "Verifying local files (%.2f%%, %.2f%% valid)",
                     tr_truncd( 100 * st->recheckProgress, 2 ),
                     tr_truncd( 100 * st->percentDone, 2 ) );
    }
    else if( st->activity == TR_STATUS_DOWNLOAD )
    {
        char upStr[80];
        char dnStr[80];
        char ratioStr[80];

        tr_formatter_speed_KBps( upStr, st->pieceUploadSpeed_KBps, sizeof( upStr ) );
        tr_formatter_speed_KBps( dnStr, st->pieceDownloadSpeed_KBps, sizeof( dnStr ) );
        tr_strlratio( ratioStr, st->ratio, sizeof( ratioStr ) );

        tr_snprintf( buf, buflen,
            "Progress: %.1f%%, "
            "dl from %d of %d peers (%s), "
            "ul to %d (%s) "
            "[%s]",
            tr_truncd( 100 * st->percentDone, 1 ),
            st->peersSendingToUs, st->peersConnected, dnStr,
            st->peersGettingFromUs, upStr,
            ratioStr );
            update_status_file(st, tor);
    }
    else if( st->activity == TR_STATUS_SEED )
    {
        char upStr[80];
        char ratioStr[80];

        tr_formatter_speed_KBps( upStr, st->pieceUploadSpeed_KBps, sizeof( upStr ) );
        tr_strlratio( ratioStr, st->ratio, sizeof( ratioStr ) );

        tr_snprintf( buf, buflen,
                     "Seeding, uploading to %d of %d peer(s), %s [%s]",
                     st->peersGettingFromUs, st->peersConnected, upStr, ratioStr );
    }
    else *buf = '\0';
}

static const char*
getConfigDir( int argc, const char ** argv )
{
    int c;
    const char * configDir = NULL;
    const char * optarg;
    const int ind = tr_optind;

    while(( c = tr_getopt( getUsage( ), argc, argv, options, &optarg ))) {
        if( c == 'g' ) {
            configDir = optarg;
            break;
        }
    }

    tr_optind = ind;

    if( configDir == NULL )
        configDir = tr_getDefaultConfigDir( MY_CONFIG_NAME );

    return configDir;
}

int
main( int argc, char ** argv )
{
    int           error;
    tr_session  * h;
    tr_ctor     * ctor;
    tr_torrent  * tor = NULL;
    tr_benc       settings;
    const char  * configDir;
    uint8_t     * fileContents;
    size_t        fileLength;
    const char  * str;
    char          buf[TR_PATH_MAX];

    tr_formatter_mem_init( MEM_K, MEM_K_STR, MEM_M_STR, MEM_G_STR, MEM_T_STR );
    tr_formatter_size_init( DISK_K,DISK_K_STR, DISK_M_STR, DISK_G_STR, DISK_T_STR );
    tr_formatter_speed_init( SPEED_K, SPEED_K_STR, SPEED_M_STR, SPEED_G_STR, SPEED_T_STR );

    printf( "%s %s\n", MY_READABLE_NAME, LONG_VERSION_STRING );

    /* user needs to pass in at least one argument */
    if( argc < 2 ) {
        tr_getopt_usage( MY_READABLE_NAME, getUsage( ), options );
        return EXIT_FAILURE;
    }

    /* load the defaults from config file + libtransmission defaults */
    tr_bencInitDict( &settings, 0 );
    configDir = getConfigDir( argc, (const char**)argv );
    tr_sessionLoadSettings( &settings, configDir, MY_CONFIG_NAME );

    /* the command line overrides defaults */
    if( parseCommandLine( &settings, argc, (const char**)argv ) )
        return EXIT_FAILURE;

    if( showVersion )
        return 0;

    /* Check the options for validity */
    if( !torrentPath ) {
        fprintf( stderr, "No torrent specified!\n" );
        return EXIT_FAILURE;
    }

    if( tr_bencDictFindStr( &settings, TR_PREFS_KEY_DOWNLOAD_DIR, &str ) ) {
        str = tr_realpath( str, buf );
        if( str != NULL ) {
            tr_bencDictAddStr( &settings, TR_PREFS_KEY_DOWNLOAD_DIR, str );
        } else {
            fprintf( stderr, "Download directory does not exist!\n" );
            return EXIT_FAILURE;
        }
    }

    h = tr_sessionInit( "cli", configDir, false, &settings );

    ctor = tr_ctorNew( h );

    fileContents = tr_loadFile( torrentPath, &fileLength );
    tr_ctorSetPaused( ctor, TR_FORCE, false );
    if( fileContents != NULL ) {
        tr_ctorSetMetainfo( ctor, fileContents, fileLength );
    } else if( !memcmp( torrentPath, "magnet:?", 8 ) ) {
        tr_ctorSetMetainfoFromMagnetLink( ctor, torrentPath );
    } else if( !memcmp( torrentPath, "http", 4 ) ) {
        tr_webRun( h, torrentPath, NULL, NULL, onTorrentFileDownloaded, ctor );
        waitingOnWeb = true;
        while( waitingOnWeb ) tr_wait_msec( 1000 );
    } else {
        fprintf( stderr, "ERROR: Unrecognized torrent \"%s\".\n", torrentPath );
        fprintf( stderr, " * If you're trying to create a torrent, use transmission-create.\n" );
        fprintf( stderr, " * If you're trying to see a torrent's info, use transmission-show.\n" );
        tr_sessionClose( h );
        return EXIT_FAILURE;
    }
    tr_free( fileContents );
/* Foxconn add start,edward zhang, 2012/11/23 @add the option Show torrent details */
    if( showInfo )
    {
        tr_info info;

        if( !tr_torrentParse( ctor, &info ) )
        {
            fflush(stdout);
            dumpInfo(&info );
            tr_metainfoFree( &info );
        }

        tr_ctorFree( ctor );
    	tr_bencFree( &settings );
    	tr_sessionClose( h );
    	return EXIT_SUCCESS;
    }
/* Foxconn add end,edward zhang, 2012/11/23 @add the option Show torrent details */
    
    tor = tr_torrentNew( ctor, &error );
    tr_ctorFree( ctor );
    if( !tor )
    {
        fprintf( stderr, "Failed opening torrent file `%s'\n", torrentPath );
        tr_sessionClose( h );
        return EXIT_FAILURE;
    }
    {
		FILE *fp;
    	fp = fopen(PID_FILE, "wt");
    	if (fp) {
        	fprintf(fp, "%d", getpid());
        	fclose(fp);
    	}else
        	perror("Can't open PID file");
        char status_path[64];
        char cmd[64];
        sprintf(status_path,"/var/run/down/mission_%d",getpid());
        sprintf(cmd,"mkdir -p %s",status_path);
        system(cmd);
        sprintf(ram_status_file,"%s/status",status_path);
    }

    signal( SIGINT, sigHandler );
    signal( SIGTERM, sigHandler );
#ifndef WIN32
    signal( SIGHUP, sigHandler );
#endif
    tr_torrentStart( tor );

    if( verify )
    {
        verify = 0;
        tr_torrentVerify( tor );
    }

    for( ; ; )
    {
        char            line[LINEWIDTH];
        const tr_stat * st;
        const char * messageName[] = { NULL, "Tracker gave a warning:",
                                             "Tracker gave an error:",
                                             "Error:" };

        tr_wait_msec( 200 );

        if( gotsig )
        {
            gotsig = 0;
            printf( "\nStopping torrent...\n" );
            tr_torrentStop( tor );
            if(stop_process == 2)
            	update_status_file(st,tor);
        }

        if( manualUpdate )
        {
            manualUpdate = 0;
            if( !tr_torrentCanManualUpdate( tor ) )
                fprintf(
                    stderr,
                    "\nReceived SIGHUP, but can't send a manual update now\n" );
            else
            {
                fprintf( stderr,
                         "\nReceived SIGHUP: manual update scheduled\n" );
                tr_torrentManualUpdate( tor );
            }
        }

        st = tr_torrentStat( tor );
        if( st->activity == TR_STATUS_STOPPED )
        {
            printf( "\nstatus change to stop, exit\n" );
            update_status_file(st,tor);
            break;
        }
        else if( st->activity == TR_STATUS_SEED )
        {
            printf( "\nstatus change to seed,stop torrent and exit\n" );
            update_status_file(st,tor);
            tr_torrentStop( tor );
            break;
        }

        getStatusStr( st, line, sizeof( line ), tor );
        printf( "\r%-*s", LINEWIDTH, line );

        //if( messageName[st->error] )
        //    fprintf( stderr, "\n%s: %s\n", messageName[st->error], st->errorString );
    }

    tr_sessionSaveSettings( h, configDir, &settings );

    printf( "\n" );
    tr_bencFree( &settings );
    tr_sessionClose( h );
    return EXIT_SUCCESS;
}

/***
****
****
****
***/

static int
parseCommandLine( tr_benc * d, int argc, const char ** argv )
{
    int          c;
    const char * optarg;

    while(( c = tr_getopt( getUsage( ), argc, argv, options, &optarg )))
    {
        switch( c )
        {
            case 'b': tr_bencDictAddBool( d, TR_PREFS_KEY_BLOCKLIST_ENABLED, true );
                      break;
            case 'B': tr_bencDictAddBool( d, TR_PREFS_KEY_BLOCKLIST_ENABLED, false );
                      break;
            case 'd': tr_bencDictAddInt ( d, TR_PREFS_KEY_DSPEED_KBps, atoi( optarg ) );
                      tr_bencDictAddBool( d, TR_PREFS_KEY_DSPEED_ENABLED, true );
                      break;
            case 'D': tr_bencDictAddBool( d, TR_PREFS_KEY_DSPEED_ENABLED, false );
                      break;
            case 'f': tr_bencDictAddStr( d, TR_PREFS_KEY_SCRIPT_TORRENT_DONE_FILENAME, optarg );
                      tr_bencDictAddBool( d, TR_PREFS_KEY_SCRIPT_TORRENT_DONE_ENABLED, true );
                      break;
            case 'g': /* handled above */
                      break;
            case 'i': showInfo = 1;
                      break;
            case 'm': tr_bencDictAddBool( d, TR_PREFS_KEY_PORT_FORWARDING, true );
                      break;
            case 'M': tr_bencDictAddBool( d, TR_PREFS_KEY_PORT_FORWARDING, false );
                      break;
            case 'p': tr_bencDictAddInt( d, TR_PREFS_KEY_PEER_PORT, atoi( optarg ) );
                      break;
            case 't': tr_bencDictAddInt( d, TR_PREFS_KEY_PEER_SOCKET_TOS, atoi( optarg ) );
                      break;
            case 'u': tr_bencDictAddInt( d, TR_PREFS_KEY_USPEED_KBps, atoi( optarg ) );
                      tr_bencDictAddBool( d, TR_PREFS_KEY_USPEED_ENABLED, true );
                      break;
            case 'U': tr_bencDictAddBool( d, TR_PREFS_KEY_USPEED_ENABLED, false );
                      break;
            case 'v': verify = true;
                      break;
            case 'V': showVersion = true;
                      break;
            case 'w': tr_bencDictAddStr( d, TR_PREFS_KEY_DOWNLOAD_DIR, optarg );
                      break;
            case 910: tr_bencDictAddInt( d, TR_PREFS_KEY_ENCRYPTION, TR_ENCRYPTION_REQUIRED );
                      break;
            case 911: tr_bencDictAddInt( d, TR_PREFS_KEY_ENCRYPTION, TR_ENCRYPTION_PREFERRED );
                      break;
            case 912: tr_bencDictAddInt( d, TR_PREFS_KEY_ENCRYPTION, TR_CLEAR_PREFERRED );
                      break;
            case TR_OPT_UNK:
                      if( torrentPath == NULL )
                          torrentPath = optarg;
                      break;
            default: return 1;
        }
    }

    return 0;
}

static void
sigHandler( int signal )
{
    switch( signal )
    {
        case SIGINT:
            gotsig = 1;
            stop_process = 1;
            break;

        case SIGTERM:
            gotsig = 1;
            stop_process = 2;
            break;
#ifndef WIN32
        case SIGHUP:
            manualUpdate = 1; break;

#endif
        default:
            break;
    }
}

