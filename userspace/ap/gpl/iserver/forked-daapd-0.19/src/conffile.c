/*
 * Copyright (C) 2009-2011 Julien BLACHE <jb@jblache.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/utsname.h>
#include <pwd.h>

#include <errno.h>

#include <confuse.h>

#include "logger.h"
#include "misc.h"
#include "conffile.h"


/* Forward */
static int cb_loglevel(cfg_t *cfg, cfg_opt_t *opt, const char *value, void *result);

/* general section structure */
static cfg_opt_t sec_general[] =
  {
    CFG_STR("uid", "nobody", CFGF_NONE),
    CFG_STR("admin_password", NULL, CFGF_NONE),
    CFG_STR("logfile", STATEDIR "/log/" PACKAGE ".log", CFGF_NONE),
    CFG_STR("db_path", STATEDIR "/cache/" PACKAGE "/songs3.db", CFGF_NONE),
    CFG_INT("db_pragma_cache_size", -1, CFGF_NONE),
    CFG_STR("db_pragma_journal_mode", NULL, CFGF_NONE),
    CFG_INT("db_pragma_synchronous", -1, CFGF_NONE),
    CFG_INT_CB("loglevel", E_LOG, CFGF_NONE, &cb_loglevel),
    CFG_BOOL("ipv6", cfg_false, CFGF_NONE),
    CFG_STR("cache_path", STATEDIR "/cache/" PACKAGE "/cache.db", CFGF_NONE),
    CFG_INT("cache_daap_threshold", 1000, CFGF_NONE),
    CFG_END()
  };

/* library section structure */
static cfg_opt_t sec_library[] =
  {
    CFG_STR("name", "My Music on %h", CFGF_NONE),
    CFG_INT("port", 3689, CFGF_NONE),
    CFG_STR("password", NULL, CFGF_NONE),
    CFG_STR_LIST("directories", NULL, CFGF_NONE),
    CFG_STR_LIST("podcasts", NULL, CFGF_NONE),
    CFG_STR_LIST("audiobooks", NULL, CFGF_NONE),
    CFG_STR_LIST("compilations", NULL, CFGF_NONE),
    CFG_STR("compilation_artist", NULL, CFGF_NONE),
    CFG_BOOL("radio_playlists", cfg_false, CFGF_NONE),
    CFG_STR("name_library", "Library", CFGF_NONE),
    CFG_STR("name_music", "Music", CFGF_NONE),
    CFG_STR("name_movies", "Movies", CFGF_NONE),
    CFG_STR("name_tvshows", "TV Shows", CFGF_NONE),
    CFG_STR("name_podcasts", "Podcasts", CFGF_NONE),
    CFG_STR("name_audiobooks", "Audiobooks", CFGF_NONE),
    CFG_STR("name_radio", "Radio", CFGF_NONE),
    CFG_STR_LIST("artwork_basenames", "{artwork,cover,Folder}", CFGF_NONE),
    CFG_BOOL("artwork_individual", cfg_false, CFGF_NONE),
    CFG_STR_LIST("filetypes_ignore", "{.db,.ini,.db-journal,.pdf}", CFGF_NONE),
    CFG_STR_LIST("filepath_ignore", NULL, CFGF_NONE),
    CFG_BOOL("filescan_disable", cfg_false, CFGF_NONE),
    CFG_BOOL("itunes_overrides", cfg_false, CFGF_NONE),
    CFG_STR_LIST("no_transcode", NULL, CFGF_NONE),
    CFG_STR_LIST("force_transcode", NULL, CFGF_NONE),
    CFG_END()
  };

/* local audio section structure */
static cfg_opt_t sec_audio[] =
  {
    CFG_STR("nickname", "Computer", CFGF_NONE),
#ifdef __linux__
    CFG_STR("card", "default", CFGF_NONE),
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
    CFG_STR("card", "/dev/dsp", CFGF_NONE),
#endif
    CFG_STR("mixer", NULL, CFGF_NONE),
    CFG_END()
  };

/* AirPlay/ApEx device section structure */
static cfg_opt_t sec_airplay[] =
  {
    CFG_INT("max_volume", 11, CFGF_NONE),
    CFG_STR("password", NULL, CFGF_NONE),
    CFG_END()
  };

/* Spotify section structure */
static cfg_opt_t sec_spotify[] =
  {
    CFG_STR("settings_dir", STATEDIR "/cache/" PACKAGE "/libspotify", CFGF_NONE),
    CFG_STR("cache_dir", "/tmp", CFGF_NONE),
    CFG_INT("bitrate", 0, CFGF_NONE),
    CFG_BOOL("base_playlist_disable", cfg_false, CFGF_NONE),
    CFG_BOOL("artist_override", cfg_false, CFGF_NONE),
    CFG_BOOL("starred_artist_override", cfg_false, CFGF_NONE),
    CFG_BOOL("album_override", cfg_false, CFGF_NONE),
    CFG_BOOL("starred_album_override", cfg_false, CFGF_NONE),
    CFG_END()
  };

/* SQLite section structure */
static cfg_opt_t sec_sqlite[] =
  {
    CFG_INT("pragma_cache_size_library", -1, CFGF_NONE),
    CFG_INT("pragma_cache_size_cache", -1, CFGF_NONE),
    CFG_STR("pragma_journal_mode", NULL, CFGF_NONE),
    CFG_INT("pragma_synchronous", -1, CFGF_NONE),
    CFG_BOOL("vacuum", cfg_true, CFGF_NONE),
    CFG_END()
  };

/* MPD section structure */
static cfg_opt_t sec_mpd[] =
  {
    CFG_INT("port", 6600, CFGF_NONE),
    CFG_END()
  };

/* Config file structure */
static cfg_opt_t toplvl_cfg[] =
  {
    CFG_SEC("general", sec_general, CFGF_NONE),
    CFG_SEC("library", sec_library, CFGF_NONE),
    CFG_SEC("audio", sec_audio, CFGF_NONE),
    CFG_SEC("airplay", sec_airplay, CFGF_MULTI | CFGF_TITLE),
    CFG_SEC("spotify", sec_spotify, CFGF_NONE),
    CFG_SEC("sqlite", sec_sqlite, CFGF_NONE),
    CFG_SEC("mpd", sec_mpd, CFGF_NONE),
    CFG_END()
  };

cfg_t *cfg;
uint64_t libhash;
uid_t runas_uid;
gid_t runas_gid;


static int
cb_loglevel(cfg_t *cfg, cfg_opt_t *opt, const char *value, void *result)
{
  if (strcasecmp(value, "fatal") == 0)
    *(long int *)result = E_FATAL;
  else if (strcasecmp(value, "log") == 0)
    *(long int *)result = E_LOG;
  else if (strcasecmp(value, "warning") == 0)
    *(long int *)result = E_WARN;
  else if (strcasecmp(value, "info") == 0)
    *(long int *)result = E_INFO;
  else if (strcasecmp(value, "debug") == 0)
    *(long int *)result = E_DBG;
  else if (strcasecmp(value, "spam") == 0)
    *(long int *)result = E_SPAM;
  else
    {
      DPRINTF(E_WARN, L_CONF, "Unrecognised loglevel '%s'\n", value);
      /* Default to warning */
      *(long int *)result = 1;
    }

  return 0;
}

static int
conffile_expand_libname(cfg_t *lib)
{
  char *libname;
  char *hostname;
  char *s;
  char *d;
  char *expanded;
  struct utsname sysinfo;
  size_t len;
  size_t olen;
  size_t hostlen;
  size_t verlen;
  int ret;

  libname = cfg_getstr(lib, "name");
  olen = strlen(libname);

  /* Fast path */
  s = strchr(libname, '%');
  if (!s)
    {
      libhash = murmur_hash64(libname, olen, 0);
      return 0;
    }

  /* Grab what we need */
  ret = uname(&sysinfo);
  if (ret != 0)
    {
      DPRINTF(E_WARN, L_CONF, "Could not get system name: %s\n", strerror(errno));
      hostname = "Unknown host";
    }
  else
    hostname = sysinfo.nodename;

  hostlen = strlen(hostname);
  verlen = strlen(VERSION);

  /* Compute expanded size */
  len = olen;
  s = libname;
  while (*s)
    {
      if (*s == '%')
	{
	  s++;

	  switch (*s)
	    {
	      case 'h':
		len += hostlen;
		break;

	      case 'v':
		len += verlen;
		break;
	    }
	}
      s++;
    }

  expanded = (char *)malloc(len + 1);
  if (!expanded)
    {
      DPRINTF(E_FATAL, L_CONF, "Out of memory\n");

      return -1;
    }
  memset(expanded, 0, len + 1);

  /* Do the actual expansion */
  s = libname;
  d = expanded;
  while (*s)
    {
      if (*s == '%')
	{
	  s++;

	  switch (*s)
	    {
	      case 'h':
		strcat(d, hostname);
		d += hostlen;
		break;

	      case 'v':
		strcat(d, VERSION);
		d += verlen;
		break;
	    }

	  s++;
	}
      else
	{
	  *d = *s;

	  s++;
	  d++;
	}
    }

  cfg_setstr(lib, "name", expanded);

  libhash = murmur_hash64(expanded, strlen(expanded), 0);

  free(expanded);

  return 0;
}


int
conffile_load(char *file)
{
  cfg_t *lib;
  struct passwd *pw;
  char *runas;
  int ret;

  cfg = cfg_init(toplvl_cfg, CFGF_NONE);

  ret = cfg_parse(cfg, file);

  if (ret == CFG_FILE_ERROR)
    {
      DPRINTF(E_FATAL, L_CONF, "Could not open config file %s\n", file);

      goto out_fail;
    }
  else if (ret == CFG_PARSE_ERROR)
    {
      DPRINTF(E_FATAL, L_CONF, "Parse error in config file %s\n", file);

      goto out_fail;
    }

  /* Resolve runas username */
  runas = cfg_getstr(cfg_getsec(cfg, "general"), "uid");
  pw = getpwnam("admin");
  if (!pw) 
    {
      DPRINTF(E_FATAL, L_CONF, "Could not lookup user %s: %s\n", runas, strerror(errno));

      goto out_fail;
    }

  runas_uid = pw->pw_uid;
  runas_gid = pw->pw_gid;

  lib = cfg_getsec(cfg, "library");

  if (cfg_size(lib, "directories") == 0)
    {
      DPRINTF(E_FATAL, L_CONF, "No directories specified for library\n");

      goto out_fail;
    }

  /* Do keyword expansion on library names */
  ret = conffile_expand_libname(lib);
  if (ret != 0)
    {
      DPRINTF(E_FATAL, L_CONF, "Could not expand library name\n");

      goto out_fail;
    }

  return 0;

 out_fail:
  cfg_free(cfg);

  return -1;
}

void
conffile_unload(void)
{
  cfg_free(cfg);
}
