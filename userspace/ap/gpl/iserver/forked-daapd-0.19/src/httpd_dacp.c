/*
 * Copyright (C) 2010-2011 Julien BLACHE <jb@jblache.org>
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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <regex.h>
#include <stdint.h>
#include <inttypes.h>

#if defined(HAVE_SYS_EVENTFD_H) && defined(HAVE_EVENTFD)
# define USE_EVENTFD
# include <sys/eventfd.h>
#endif

#include <event.h>
#include "evhttp/evhttp.h"

#include "logger.h"
#include "misc.h"
#include "conffile.h"
#include "artwork.h"
#include "httpd.h"
#include "httpd_dacp.h"
#include "dmap_common.h"
#include "db.h"
#include "player.h"


/* httpd event base, from httpd.c */
extern struct event_base *evbase_httpd;

/* From httpd_daap.c */
struct daap_session;

struct daap_session *
daap_session_find(struct evhttp_request *req, struct evkeyvalq *query, struct evbuffer *evbuf);


struct uri_map {
  regex_t preg;
  char *regexp;
  void (*handler)(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query);
};

struct dacp_update_request {
  struct evhttp_request *req;

  struct dacp_update_request *next;
};

typedef void (*dacp_propget)(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi);
typedef void (*dacp_propset)(const char *value, struct evkeyvalq *query);

struct dacp_prop_map {
  char *desc;
  dacp_propget propget;
  dacp_propset propset;
};


/* Forward - properties getters */
static void
dacp_propget_volume(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi);
static void
dacp_propget_volumecontrollable(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi);
static void
dacp_propget_playerstate(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi);
static void
dacp_propget_shufflestate(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi);
static void
dacp_propget_availableshufflestates(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi);
static void
dacp_propget_repeatstate(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi);
static void
dacp_propget_availablerepeatstates(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi);
static void
dacp_propget_nowplaying(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi);
static void
dacp_propget_playingtime(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi);

static void 
dacp_propget_fullscreenenabled(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi); 
static void 
dacp_propget_fullscreen(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi); 
static void 
dacp_propget_visualizerenabled(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi); 
static void 
dacp_propget_visualizer(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi); 
static void 
dacp_propget_itms_songid(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi); 
static void 
dacp_propget_haschapterdata(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi); 

/* Forward - properties setters */
static void
dacp_propset_volume(const char *value, struct evkeyvalq *query);
static void
dacp_propset_playingtime(const char *value, struct evkeyvalq *query);
static void
dacp_propset_shufflestate(const char *value, struct evkeyvalq *query);
static void
dacp_propset_repeatstate(const char *value, struct evkeyvalq *query);
static void
dacp_propset_userrating(const char *value, struct evkeyvalq *query);


/* gperf static hash, dacp_prop.gperf */
#include "dacp_prop_hash.c"


/* Play status update */
#ifdef USE_EVENTFD
static int update_efd;
#else
static int update_pipe[2];
#endif
static struct event updateev;
static int current_rev;

/* Play status update requests */
static struct dacp_update_request *update_requests;

/* Seek timer */
static struct event seek_timer;
static int seek_target;

static void 
dacp_propget_fullscreenenabled(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi) 
{ 
  // TODO 
} 

static void 
dacp_propget_fullscreen(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi) 
{ 
  // TODO 
} 

static void 
dacp_propget_visualizerenabled(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi) 
{ 
  // TODO 
} 

static void 
dacp_propget_visualizer(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi) 
{ 
  // TODO 
} 
 
static void 
dacp_propget_itms_songid(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi) 
{ 
  // TODO 
} 
 
static void 
dacp_propget_haschapterdata(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi) 
{ 
  // TODO 
} 


/* DACP helpers */
static void
dacp_nowplaying(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  char canp[16];

  if ((status->status == PLAY_STOPPED) || !mfi)
    return;

  memset(canp, 0, sizeof(canp));

  canp[3] = 1; /* 0-3 database ID */

  canp[4]  = (status->plid >> 24) & 0xff;
  canp[5]  = (status->plid >> 16) & 0xff;
  canp[6]  = (status->plid >> 8) & 0xff;
  canp[7]  = status->plid & 0xff;

  canp[8]  = (status->pos_pl >> 24) & 0xff; /* 8-11 position in playlist */
  canp[9]  = (status->pos_pl >> 16) & 0xff;
  canp[10] = (status->pos_pl >> 8) & 0xff;
  canp[11] = status->pos_pl & 0xff;

  canp[12] = (status->id >> 24) & 0xff; /* 12-15 track ID */
  canp[13] = (status->id >> 16) & 0xff;
  canp[14] = (status->id >> 8) & 0xff;
  canp[15] = status->id & 0xff;

  dmap_add_literal(evbuf, "canp", canp, sizeof(canp));

  dmap_add_string(evbuf, "cann", mfi->title);
  dmap_add_string(evbuf, "cana", mfi->artist);
  dmap_add_string(evbuf, "canl", mfi->album);
  dmap_add_string(evbuf, "cang", mfi->genre);
  dmap_add_long(evbuf, "asai", mfi->songalbumid);

  dmap_add_int(evbuf, "cmmk", 1);
}

static void
dacp_playingtime(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  if ((status->status == PLAY_STOPPED) || !mfi)
    return;

  dmap_add_int(evbuf, "cant", mfi->song_length - status->pos_ms); /* Remaining time in ms */
  dmap_add_int(evbuf, "cast", mfi->song_length); /* Song length in ms */
}


/* Update requests helpers */
static int
make_playstatusupdate(struct evbuffer *evbuf)
{
  struct player_status status;
  struct media_file_info *mfi;
  struct evbuffer *psu;
  int ret;

  psu = evbuffer_new();
  if (!psu)
    {
      DPRINTF(E_LOG, L_DACP, "Could not allocate evbuffer for playstatusupdate\n");

      return -1;
    }

  player_get_status(&status);

  if (status.status != PLAY_STOPPED)
    {
      mfi = db_file_fetch_byid(status.id);
      if (!mfi)
	{
	  DPRINTF(E_LOG, L_DACP, "Could not fetch file id %d\n", status.id);

	  return -1;
	}
    }
  else
    mfi = NULL;

  dmap_add_int(psu, "mstt", 200);         /* 12 */

  dmap_add_int(psu, "cmsr", current_rev); /* 12 */

  dmap_add_char(psu, "cavc", 1);              /* 9 */ /* volume controllable */
  dmap_add_char(psu, "caps", status.status);  /* 9 */ /* play status, 2 = stopped, 3 = paused, 4 = playing */
  dmap_add_char(psu, "cash", status.shuffle); /* 9 */ /* shuffle, true/false */
  dmap_add_char(psu, "carp", status.repeat);  /* 9 */ /* repeat, 0 = off, 1 = repeat song, 2 = repeat (playlist) */

  dmap_add_int(psu, "caas", 2);           /* 12 */ /* available shuffle states */
  dmap_add_int(psu, "caar", 6);           /* 12 */ /* available repeat states */

  if (mfi)
    {
      dacp_nowplaying(psu, &status, mfi);
      dacp_playingtime(psu, &status, mfi);

      free_mfi(mfi, 0);
    }

  dmap_add_container(evbuf, "cmst", EVBUFFER_LENGTH(psu));    /* 8 + len */

  ret = evbuffer_add_buffer(evbuf, psu);
  evbuffer_free(psu);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Could not add status data to playstatusupdate reply\n");

      return -1;
    }

  return 0;
}

static void
playstatusupdate_cb(int fd, short what, void *arg)
{
  struct dacp_update_request *ur;
  struct evbuffer *evbuf;
  struct evbuffer *update;
  int ret;

#ifdef USE_EVENTFD
  eventfd_t count;

  ret = eventfd_read(update_efd, &count);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Could not read playstatusupdate event counter: %s\n", strerror(errno));

      goto readd;
    }
#else
  int dummy;

  read(update_pipe[0], &dummy, sizeof(dummy));
#endif

  if (!update_requests)
    goto readd;

  evbuf = evbuffer_new();
  if (!evbuf)
    {
      DPRINTF(E_LOG, L_DACP, "Could not allocate evbuffer for playstatusupdate reply\n");

      goto readd;
    }

  update = evbuffer_new();
  if (!update)
    {
      DPRINTF(E_LOG, L_DACP, "Could not allocate evbuffer for playstatusupdate data\n");

      goto out_free_evbuf;
    }

  ret = make_playstatusupdate(update);
  if (ret < 0)
    goto out_free_update;

  for (ur = update_requests; update_requests; ur = update_requests)
    {
      update_requests = ur->next;

      evhttp_connection_set_closecb(ur->req->evcon, NULL, NULL);

      evbuffer_add(evbuf, EVBUFFER_DATA(update), EVBUFFER_LENGTH(update));

      httpd_send_reply(ur->req, HTTP_OK, "OK", evbuf);

      free(ur);
    }

  current_rev++;

 out_free_update:
  evbuffer_free(update);
 out_free_evbuf:
  evbuffer_free(evbuf);
 readd:
  ret = event_add(&updateev, NULL);
  if (ret < 0)
    DPRINTF(E_LOG, L_DACP, "Couldn't re-add event for playstatusupdate\n");
}

/* Thread: player */
static void
dacp_playstatus_update_handler(void)
{
  int ret;

#ifdef USE_EVENTFD
  ret = eventfd_write(update_efd, 1);
  if (ret < 0)
    DPRINTF(E_LOG, L_DACP, "Could not send status update event: %s\n", strerror(errno));
#else
  int dummy = 42;

  ret = write(update_pipe[1], &dummy, sizeof(dummy));
  if (ret != sizeof(dummy))
    DPRINTF(E_LOG, L_DACP, "Could not write to status update fd: %s\n", strerror(errno));
#endif
}

static void
update_fail_cb(struct evhttp_connection *evcon, void *arg)
{
  struct dacp_update_request *ur;
  struct dacp_update_request *p;

  ur = (struct dacp_update_request *)arg;

  DPRINTF(E_DBG, L_DACP, "Update request: client closed connection\n");

  if (ur->req->evcon)
    evhttp_connection_set_closecb(ur->req->evcon, NULL, NULL);

  if (ur == update_requests)
    update_requests = ur->next;
  else
    {
      for (p = update_requests; p && (p->next != ur); p = p->next)
	;

      if (!p)
	{
	  DPRINTF(E_LOG, L_DACP, "WARNING: struct dacp_update_request not found in list; BUG!\n");
	  return;
	}

      p->next = ur->next;
    }

  free(ur);
}


/* Properties getters */
static void
dacp_propget_volume(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  dmap_add_int(evbuf, "cmvo", status->volume);
}

static void
dacp_propget_volumecontrollable(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  dmap_add_char(evbuf, "cavc", 1);
}

static void
dacp_propget_playerstate(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  dmap_add_char(evbuf, "caps", status->status);
}

static void
dacp_propget_shufflestate(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  dmap_add_char(evbuf, "cash", status->shuffle);
}

static void
dacp_propget_availableshufflestates(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  dmap_add_int(evbuf, "caas", 2);
}

static void
dacp_propget_repeatstate(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  dmap_add_char(evbuf, "carp", status->repeat);
}

static void
dacp_propget_availablerepeatstates(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  dmap_add_int(evbuf, "caar", 6);
}

static void
dacp_propget_nowplaying(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  dacp_nowplaying(evbuf, status, mfi);
}

static void
dacp_propget_playingtime(struct evbuffer *evbuf, struct player_status *status, struct media_file_info *mfi)
{
  dacp_playingtime(evbuf, status, mfi);
}

/* Properties setters */
static void
dacp_propset_volume(const char *value, struct evkeyvalq *query)
{
  const char *param;
  uint64_t id;
  int volume;
  int ret;

  ret = safe_atoi32(value, &volume);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "dmcp.volume argument doesn't convert to integer: %s\n", value);

      return;
    }

  param = evhttp_find_header(query, "speaker-id");
  if (param)
    {
      ret = safe_atou64(param, &id);
      if (ret < 0)
	{
	  DPRINTF(E_LOG, L_DACP, "Invalid speaker ID in dmcp.volume request\n");

	  return;
	}

      player_volume_setrel_speaker(id, volume);
      return;
    }

  param = evhttp_find_header(query, "include-speaker-id");
  if (param)
    {
      ret = safe_atou64(param, &id);
      if (ret < 0)
	{
	  DPRINTF(E_LOG, L_DACP, "Invalid speaker ID in dmcp.volume request\n");

	  return;
	}

      player_volume_setabs_speaker(id, volume);
      return;
    }

  player_volume_set(volume);
}

static void
seek_timer_cb(int fd, short what, void *arg)
{
  int ret;

  DPRINTF(E_DBG, L_DACP, "Seek timer expired, target %d ms\n", seek_target);

  ret = player_playback_seek(seek_target);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Player failed to seek to %d ms\n", seek_target);

      return;
    }

  ret = player_playback_start(NULL);
  if (ret < 0)
    DPRINTF(E_LOG, L_DACP, "Player returned an error for start after seek\n");
}

static void
dacp_propset_playingtime(const char *value, struct evkeyvalq *query)
{
  struct timeval tv;
  int ret;

  if (event_initialized(&seek_timer))
    event_del(&seek_timer);

  ret = safe_atoi32(value, &seek_target);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "dacp.playingtime argument doesn't convert to integer: %s\n", value);

      return;
    }

  evtimer_set(&seek_timer, seek_timer_cb, NULL);
  event_base_set(evbase_httpd, &seek_timer);
  evutil_timerclear(&tv);
  tv.tv_usec = 200 * 1000;
  evtimer_add(&seek_timer, &tv);
}

static void
dacp_propset_shufflestate(const char *value, struct evkeyvalq *query)
{
  int enable;
  int ret;

  ret = safe_atoi32(value, &enable);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "dacp.shufflestate argument doesn't convert to integer: %s\n", value);

      return;
    }

  player_shuffle_set(enable);
}

static void
dacp_propset_repeatstate(const char *value, struct evkeyvalq *query)
{
  int mode;
  int ret;

  ret = safe_atoi32(value, &mode);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "dacp.repeatstate argument doesn't convert to integer: %s\n", value);

      return;
    }

  player_repeat_set(mode);
}

static void
dacp_propset_userrating(const char *value, struct evkeyvalq *query)
{
  struct media_file_info *mfi;
  const char *param;
  uint32_t itemid;
  uint32_t rating;
  int ret;

  ret = safe_atou32(value, &rating);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "dacp.userrating argument doesn't convert to integer: %s\n", value);

      return;
    }

  param = evhttp_find_header(query, "item-spec"); // Remote
  if (!param)
    param = evhttp_find_header(query, "song-spec"); // Retune

  if (!param)
    {
      DPRINTF(E_LOG, L_DACP, "Missing item-spec/song-spec parameter in dacp.userrating query\n");

      return;
    }

  param = strchr(param, ':');
  if (!param)
    {
      DPRINTF(E_LOG, L_DACP, "Malformed item-spec/song-spec parameter in dacp.userrating query\n");

      return;
    }

  param++;
  ret = safe_hextou32(param, &itemid);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Couldn't convert item-spec/song-spec to an integer in dacp.userrating (%s)\n", param);

      return;
    }

  mfi = db_file_fetch_byid(itemid);
  if (!mfi)
    {
      DPRINTF(E_LOG, L_DACP, "Could not fetch file id %d\n", itemid);

      return;
    }

  mfi->rating = rating;

  /* We're not touching any string field in mfi, so it's safe to
   * skip unicode_fixup_mfi() before the update
   */
  db_file_update(mfi);

  free_mfi(mfi, 0);
}


static void
dacp_reply_ctrlint(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  dmap_add_container(evbuf, "caci", 127); /* 8 + len */
  dmap_add_int(evbuf, "mstt", 200);       /* 12 */
  dmap_add_char(evbuf, "muty", 0);        /* 9 */
  dmap_add_int(evbuf, "mtco", 1);         /* 12 */
  dmap_add_int(evbuf, "mrco", 1);         /* 12 */
  dmap_add_container(evbuf, "mlcl", 125); /* 8 + len */
  dmap_add_container(evbuf, "mlit", 117); /* 8 + len */
  dmap_add_int(evbuf, "miid", 1);         /* 12 */ /* Database ID */
  dmap_add_char(evbuf, "cmik", 1);        /* 9 */

  dmap_add_int(evbuf, "cmpr", (2 << 16 | 1)); /* 12 */
  dmap_add_int(evbuf, "capr", (2 << 16 | 2)); /* 12 */

  dmap_add_char(evbuf, "cmsp", 1);        /* 9 */
  dmap_add_char(evbuf, "aeFR", 0x64);     /* 9 */
  dmap_add_char(evbuf, "cmsv", 1);        /* 9 */
  dmap_add_char(evbuf, "cass", 1);        /* 9 */
  dmap_add_char(evbuf, "caov", 1);        /* 9 */
  dmap_add_char(evbuf, "casu", 1);        /* 9 */
  dmap_add_char(evbuf, "ceSG", 1);        /* 9 */
  dmap_add_char(evbuf, "cmrl", 1);        /* 9 */

  httpd_send_reply(req, HTTP_OK, "OK", evbuf);
}

static void
dacp_reply_cue_play(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct player_status status;
  struct player_source *ps;
  const char *sort;
  const char *cuequery;
  const char *param;
  uint32_t id;
  int clear;
  int ret;

  /* /cue?command=play&query=...&sort=...&index=N */

  param = evhttp_find_header(query, "clear-first");
  if (param)
    {
      ret = safe_atoi32(param, &clear);
      if (ret < 0)
	DPRINTF(E_LOG, L_DACP, "Invalid clear-first value in cue request\n");
      else if (clear)
	{
	  player_playback_stop();

	  player_queue_clear();
	}
    }

  cuequery = evhttp_find_header(query, "query");
  if (cuequery)
    {
      sort = evhttp_find_header(query, "sort");

      ps = player_queue_make_daap(cuequery, sort);
      if (!ps)
	{
	  DPRINTF(E_LOG, L_DACP, "Could not build song queue\n");

	  dmap_send_error(req, "cacr", "Could not build song queue");
	  return;
	}

      player_queue_add(ps);
    }
  else
    {
      player_get_status(&status);

      if (status.status != PLAY_STOPPED)
	player_playback_stop();
    }

  param = evhttp_find_header(query, "dacp.shufflestate");
  if (param)
    dacp_propset_shufflestate(param, NULL);

  id = 0;
  param = evhttp_find_header(query, "index");
  if (param)
    {
      ret = safe_atou32(param, &id);
      if (ret < 0)
	DPRINTF(E_LOG, L_DACP, "Invalid index (%s) in cue request\n", param);
    }

  ret = player_playback_start(&id);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Could not start playback\n");

      dmap_send_error(req, "cacr", "Playback failed to start");
      return;
    }

  dmap_add_container(evbuf, "cacr", 24); /* 8 + len */
  dmap_add_int(evbuf, "mstt", 200);      /* 12 */
  dmap_add_int(evbuf, "miid", id);       /* 12 */

  httpd_send_reply(req, HTTP_OK, "OK", evbuf);
}

static void
dacp_reply_cue_clear(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  /* /cue?command=clear */

  player_playback_stop();

  player_queue_clear();

  dmap_add_container(evbuf, "cacr", 24); /* 8 + len */
  dmap_add_int(evbuf, "mstt", 200);      /* 12 */
  dmap_add_int(evbuf, "miid", 0);        /* 12 */

  httpd_send_reply(req, HTTP_OK, "OK", evbuf);
}

static void
dacp_reply_cue(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;
  const char *param;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  param = evhttp_find_header(query, "command");
  if (!param)
    {
      DPRINTF(E_DBG, L_DACP, "No command in cue request\n");

      dmap_send_error(req, "cacr", "No command in cue request");
      return;
    }

  if (strcmp(param, "clear") == 0)
    dacp_reply_cue_clear(req, evbuf, uri, query);
  else if (strcmp(param, "play") == 0)
    dacp_reply_cue_play(req, evbuf, uri, query);
  else
    {
      DPRINTF(E_LOG, L_DACP, "Unknown cue command %s\n", param);

      dmap_send_error(req, "cacr", "Unknown command in cue request");
      return;
    }
}

static void
dacp_reply_playspec(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct player_status status;
  struct player_source *ps;
  struct daap_session *s;
  const char *param;
  const char *shuffle;
  uint32_t plid;
  uint32_t id;
  int ret;

  /* /ctrl-int/1/playspec?database-spec='dmap.persistentid:0x1'&container-spec='dmap.persistentid:0x5'&container-item-spec='dmap.containeritemid:0x9'
   * With our DAAP implementation, container-spec is the playlist ID and container-item-spec is the song ID
   */

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  /* Check for shuffle */
  shuffle = evhttp_find_header(query, "dacp.shufflestate");

  /* Playlist ID */
  param = evhttp_find_header(query, "container-spec");
  if (!param)
    {
      DPRINTF(E_LOG, L_DACP, "No container-spec in playspec request\n");

      goto out_fail;
    }

  param = strchr(param, ':');
  if (!param)
    {
      DPRINTF(E_LOG, L_DACP, "Malformed container-spec parameter in playspec request\n");

      goto out_fail;
    }
  param++;

  ret = safe_hextou32(param, &plid);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Couldn't convert container-spec to an integer in playspec (%s)\n", param);

      goto out_fail;
    }

  if (!shuffle)
    {
      /* Start song ID */
      param = evhttp_find_header(query, "container-item-spec");
      if (!param)
	{
	  DPRINTF(E_LOG, L_DACP, "No container-item-spec in playspec request\n");

	  goto out_fail;
	}

      param = strchr(param, ':');
      if (!param)
	{
	  DPRINTF(E_LOG, L_DACP, "Malformed container-item-spec parameter in playspec request\n");

	  goto out_fail;
	}
      param++;

      ret = safe_hextou32(param, &id);
      if (ret < 0)
	{
	  DPRINTF(E_LOG, L_DACP, "Couldn't convert container-item-spec to an integer in playspec (%s)\n", param);

	  goto out_fail;
	}
    }
  else
    id = 0;

  DPRINTF(E_DBG, L_DACP, "Playspec request for playlist %d, start song id %d%s\n", plid, id, (shuffle) ? ", shuffle" : "");

  ps = player_queue_make_pl(plid, &id);
  if (!ps)
    {
      DPRINTF(E_LOG, L_DACP, "Could not build song queue from playlist %d\n", plid);

      goto out_fail;
    }

  DPRINTF(E_DBG, L_DACP, "Playspec start song index is %d\n", id);

  player_get_status(&status);

  if (status.status != PLAY_STOPPED)
    player_playback_stop();

  player_queue_clear();
  player_queue_add(ps);
  player_queue_plid(plid);

  if (shuffle)
    dacp_propset_shufflestate(shuffle, NULL);

  ret = player_playback_start(&id);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Could not start playback\n");

      goto out_fail;
    }

  /* 204 No Content is the canonical reply */
  evhttp_send_reply(req, HTTP_NOCONTENT, "No Content", evbuf);
  return;

 out_fail:
  evhttp_send_error(req, 500, "Internal Server Error");
}

static void
dacp_reply_pause(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  player_playback_pause();

  /* 204 No Content is the canonical reply */
  evhttp_send_reply(req, HTTP_NOCONTENT, "No Content", evbuf);
}

static void
dacp_reply_playpause(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;
  int ret;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  ret = player_playback_start(NULL);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Player returned an error for start after pause\n");

      evhttp_send_error(req, 500, "Internal Server Error");
      return;
    }

  /* 204 No Content is the canonical reply */
  evhttp_send_reply(req, HTTP_NOCONTENT, "No Content", evbuf);
}

static void
dacp_reply_nextitem(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;
  int ret;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  ret = player_playback_next();
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Player returned an error for nextitem\n");

      evhttp_send_error(req, 500, "Internal Server Error");
      return;
    }

  ret = player_playback_start(NULL);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Player returned an error for start after nextitem\n");

      evhttp_send_error(req, 500, "Internal Server Error");
      return;
    }

  /* 204 No Content is the canonical reply */
  evhttp_send_reply(req, HTTP_NOCONTENT, "No Content", evbuf);
}

static void
dacp_reply_previtem(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;
  int ret;

  /*
   * Support (Android) remotes that always send playpause to pause or start a
   * song.
   */
  if(player_is_playing())
    return dacp_reply_pause(req, evbuf, uri, query);


  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  ret = player_playback_prev();
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Player returned an error for previtem\n");

      evhttp_send_error(req, 500, "Internal Server Error");
      return;
    }

  ret = player_playback_start(NULL);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Player returned an error for start after previtem\n");

      evhttp_send_error(req, 500, "Internal Server Error");
      return;
    }

  /* 204 No Content is the canonical reply */
  evhttp_send_reply(req, HTTP_NOCONTENT, "No Content", evbuf);
}

static void
dacp_reply_beginff(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  /* TODO */

  /* 204 No Content is the canonical reply */
  evhttp_send_reply(req, HTTP_NOCONTENT, "No Content", evbuf);
}

static void
dacp_reply_beginrew(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  /* TODO */

  /* 204 No Content is the canonical reply */
  evhttp_send_reply(req, HTTP_NOCONTENT, "No Content", evbuf);
}

static void
dacp_reply_playresume(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  /* TODO */

  /* 204 No Content is the canonical reply */
  evhttp_send_reply(req, HTTP_NOCONTENT, "No Content", evbuf);
}

static void
dacp_reply_playstatusupdate(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;
  struct dacp_update_request *ur;
  const char *param;
  int reqd_rev;
  int ret;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  param = evhttp_find_header(query, "revision-number");
  if (!param)
    {
      DPRINTF(E_LOG, L_DACP, "Missing revision-number in update request\n");

      dmap_send_error(req, "cmst", "Invalid request");
      return;
    }

  ret = safe_atoi32(param, &reqd_rev);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Parameter revision-number not an integer\n");

      dmap_send_error(req, "cmst", "Invalid request");
      return;
    }

  if (reqd_rev == 1)
    {
      ret = make_playstatusupdate(evbuf);
      if (ret < 0)
	evhttp_send_error(req, 500, "Internal Server Error");
      else
	httpd_send_reply(req, HTTP_OK, "OK", evbuf);

      return;
    }

  /* Else, just let the request hang until we have changes to push back */
  ur = (struct dacp_update_request *)malloc(sizeof(struct dacp_update_request));
  if (!ur)
    {
      DPRINTF(E_LOG, L_DACP, "Out of memory for update request\n");

      dmap_send_error(req, "cmst", "Out of memory");
      return;
    }

  ur->req = req;

  ur->next = update_requests;
  update_requests = ur;

  /* If the connection fails before we have an update to push out
   * to the client, we need to know.
   */
  evhttp_connection_set_closecb(req->evcon, update_fail_cb, ur);
}

static void
dacp_reply_nowplayingartwork(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  char clen[32];
  struct daap_session *s;
  const char *param;
  char *ctype;
  uint32_t id;
  int max_w;
  int max_h;
  int ret;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  param = evhttp_find_header(query, "mw");
  if (!param)
    {
      DPRINTF(E_LOG, L_DACP, "Request for artwork without mw parameter\n");

      evhttp_send_error(req, HTTP_BADREQUEST, "Bad Request");
      return;
    }

  ret = safe_atoi32(param, &max_w);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Could not convert mw parameter to integer\n");

      evhttp_send_error(req, HTTP_BADREQUEST, "Bad Request");
      return;
    }

  param = evhttp_find_header(query, "mh");
  if (!param)
    {
      DPRINTF(E_LOG, L_DACP, "Request for artwork without mh parameter\n");

      evhttp_send_error(req, HTTP_BADREQUEST, "Bad Request");
      return;
    }

  ret = safe_atoi32(param, &max_h);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Could not convert mh parameter to integer\n");

      evhttp_send_error(req, HTTP_BADREQUEST, "Bad Request");
      return;
    }

  ret = player_now_playing(&id);
  if (ret < 0)
    goto no_artwork;

  ret = artwork_get_item(id, max_w, max_h, ART_CAN_PNG | ART_CAN_JPEG, evbuf);
  switch (ret)
    {
      case ART_FMT_PNG:
	ctype = "image/png";
	break;

      case ART_FMT_JPEG:
	ctype = "image/jpeg";
	break;

      default:
	if (EVBUFFER_LENGTH(evbuf) > 0)
	  evbuffer_drain(evbuf, EVBUFFER_LENGTH(evbuf));

	goto no_artwork;
    }

  evhttp_remove_header(req->output_headers, "Content-Type");
  evhttp_add_header(req->output_headers, "Content-Type", ctype);
  snprintf(clen, sizeof(clen), "%ld", (long)EVBUFFER_LENGTH(evbuf));
  evhttp_add_header(req->output_headers, "Content-Length", clen);

  /* No gzip compression for artwork */
  evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
  return;

 no_artwork:
  evhttp_send_error(req, HTTP_NOTFOUND, "Not Found");
}

static void
dacp_reply_getproperty(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct player_status status;
  struct daap_session *s;
  const struct dacp_prop_map *dpm;
  struct media_file_info *mfi;
  struct evbuffer *proplist;
  const char *param;
  char *ptr;
  char *prop;
  char *propstr;
  int ret;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  param = evhttp_find_header(query, "properties");
  if (!param)
    {
      DPRINTF(E_WARN, L_DACP, "Invalid DACP getproperty request, no properties\n");

      dmap_send_error(req, "cmgt", "Invalid request");
      return;
    }

  propstr = strdup(param);
  if (!propstr)
    {
      DPRINTF(E_LOG, L_DACP, "Could not duplicate properties parameter; out of memory\n");

      dmap_send_error(req, "cmgt", "Out of memory");
      return;
    }

  proplist = evbuffer_new();
  if (!proplist)
    {
      DPRINTF(E_LOG, L_DACP, "Could not allocate evbuffer for properties list\n");

      dmap_send_error(req, "cmgt", "Out of memory");
      goto out_free_propstr;
    }

  player_get_status(&status);

  if (status.status != PLAY_STOPPED)
    {
      mfi = db_file_fetch_byid(status.id);
      if (!mfi)
	{
	  DPRINTF(E_LOG, L_DACP, "Could not fetch file id %d\n", status.id);

	  dmap_send_error(req, "cmgt", "Server error");
	  goto out_free_proplist;
	}
    }
  else
    mfi = NULL;

  prop = strtok_r(propstr, ",", &ptr);
  while (prop)
    {
      dpm = dacp_find_prop(prop, strlen(prop));
      if (dpm)
	{
	  if (dpm->propget)
	    dpm->propget(proplist, &status, mfi);
	  else
	    DPRINTF(E_WARN, L_DACP, "No getter method for DACP property %s\n", prop);
	}
      else
	DPRINTF(E_LOG, L_DACP, "Could not find requested property '%s'\n", prop);

      prop = strtok_r(NULL, ",", &ptr);
    }

  free(propstr);

  if (mfi)
    free_mfi(mfi, 0);

  dmap_add_container(evbuf, "cmgt", 12 + EVBUFFER_LENGTH(proplist)); /* 8 + len */
  dmap_add_int(evbuf, "mstt", 200);      /* 12 */

  ret = evbuffer_add_buffer(evbuf, proplist);
  evbuffer_free(proplist);
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Could not add properties to getproperty reply\n");

      dmap_send_error(req, "cmgt", "Out of memory");
      return;
    }

  httpd_send_reply(req, HTTP_OK, "OK", evbuf);

  return;

 out_free_proplist:
  evbuffer_free(proplist);

 out_free_propstr:
  free(propstr);
}

static void
dacp_reply_setproperty(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;
  const struct dacp_prop_map *dpm;
  struct evkeyval *param;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  /* Known properties:
   * dacp.shufflestate 0/1
   * dacp.repeatstate  0/1/2
   * dacp.playingtime  seek to time in ms
   * dmcp.volume       0-100, float
   */

  /* /ctrl-int/1/setproperty?dacp.shufflestate=1&session-id=100 */

  TAILQ_FOREACH(param, query, next)
    {
      dpm = dacp_find_prop(param->key, strlen(param->key));

      if (!dpm)
	{
	  DPRINTF(E_SPAM, L_DACP, "Unknown DACP property %s\n", param->key);
	  continue;
	}

      if (dpm->propset)
	dpm->propset(param->value, query);
      else
	DPRINTF(E_WARN, L_DACP, "No setter method for DACP property %s\n", dpm->desc);
    }

  /* 204 No Content is the canonical reply */
  evhttp_send_reply(req, HTTP_NOCONTENT, "No Content", evbuf);
}

static void
speaker_enum_cb(uint64_t id, const char *name, int relvol, struct spk_flags flags, void *arg)
{
  struct evbuffer *evbuf;
  int len;

  evbuf = (struct evbuffer *)arg;

  len = 8 + strlen(name) + 28;
  if (flags.selected)
    len += 9;
  if (flags.has_password)
    len += 9;
  if (flags.has_video)
    len += 9;

  dmap_add_container(evbuf, "mdcl", len); /* 8 + len */
  if (flags.selected)
    dmap_add_char(evbuf, "caia", 1);      /* 9 */
  if (flags.has_password)
    dmap_add_char(evbuf, "cahp", 1);      /* 9 */
  if (flags.has_video)
    dmap_add_char(evbuf, "caiv", 1);      /* 9 */
  dmap_add_string(evbuf, "minm", name);   /* 8 + len */
  dmap_add_long(evbuf, "msma", id);       /* 16 */

  dmap_add_int(evbuf, "cmvo", relvol);    /* 12 */
}

static void
dacp_reply_getspeakers(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;
  struct evbuffer *spklist;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  spklist = evbuffer_new();
  if (!spklist)
    {
      DPRINTF(E_LOG, L_DACP, "Could not create evbuffer for speaker list\n");

      dmap_send_error(req, "casp", "Out of memory");

      return;
    }

  player_speaker_enumerate(speaker_enum_cb, spklist);

  dmap_add_container(evbuf, "casp", 12 + EVBUFFER_LENGTH(spklist)); /* 8 + len */
  dmap_add_int(evbuf, "mstt", 200); /* 12 */

  evbuffer_add_buffer(evbuf, spklist);

  evbuffer_free(spklist);

  httpd_send_reply(req, HTTP_OK, "OK", evbuf);
}

static void
dacp_reply_setspeakers(struct evhttp_request *req, struct evbuffer *evbuf, char **uri, struct evkeyvalq *query)
{
  struct daap_session *s;
  const char *param;
  const char *ptr;
  uint64_t *ids;
  int nspk;
  int i;
  int ret;

  s = daap_session_find(req, query, evbuf);
  if (!s)
    return;

  param = evhttp_find_header(query, "speaker-id");
  if (!param)
    {
      DPRINTF(E_LOG, L_DACP, "Missing speaker-id parameter in DACP setspeakers request\n");

      evhttp_send_error(req, HTTP_BADREQUEST, "Bad Request");
      return;
    }

  if (strlen(param) == 0)
    {
      ids = NULL;
      goto fastpath;
    }

  nspk = 1;
  ptr = param;
  while ((ptr = strchr(ptr + 1, ',')))
    nspk++;

  ids = (uint64_t *)malloc((nspk + 1) * sizeof(uint64_t));
  if (!ids)
    {
      DPRINTF(E_LOG, L_DACP, "Out of memory for speaker ids\n");

      evhttp_send_error(req, HTTP_SERVUNAVAIL, "Internal Server Error");
      return;
    }

  param--;
  i = 1;
  do
    {
      param++;
      ret = safe_hextou64(param, &ids[i]);
      if (ret < 0)
	{
	  DPRINTF(E_LOG, L_DACP, "Invalid speaker id in request: %s\n", param);

	  nspk--;
	  continue;
	}

      i++;
    }
  while ((param = strchr(param + 1, ',')));

  ids[0] = nspk;

 fastpath:
  ret = player_speaker_set(ids);

  if (ids)
    free(ids);

  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Speakers de/activation failed!\n");

      /* Password problem */
      if (ret == -2)
	evhttp_send_error(req, 902, "");
      else
	evhttp_send_error(req, 500, "Internal Server Error");

      return;
    }

  /* 204 No Content is the canonical reply */
  evhttp_send_reply(req, HTTP_NOCONTENT, "No Content", evbuf);
}


static struct uri_map dacp_handlers[] =
  {
    {
      .regexp = "^/ctrl-int$",
      .handler = dacp_reply_ctrlint
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/cue$",
      .handler = dacp_reply_cue
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/playspec$",
      .handler = dacp_reply_playspec
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/pause$",
      .handler = dacp_reply_pause
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/playpause$",
      .handler = dacp_reply_playpause
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/nextitem$",
      .handler = dacp_reply_nextitem
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/previtem$",
      .handler = dacp_reply_previtem
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/beginff$",
      .handler = dacp_reply_beginff
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/beginrew$",
      .handler = dacp_reply_beginrew
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/playresume$",
      .handler = dacp_reply_playresume
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/playstatusupdate$",
      .handler = dacp_reply_playstatusupdate
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/nowplayingartwork$",
      .handler = dacp_reply_nowplayingartwork
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/getproperty$",
      .handler = dacp_reply_getproperty
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/setproperty$",
      .handler = dacp_reply_setproperty
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/getspeakers$",
      .handler = dacp_reply_getspeakers
    },
    {
      .regexp = "^/ctrl-int/[[:digit:]]+/setspeakers$",
      .handler = dacp_reply_setspeakers
    },
    {
      .regexp = NULL,
      .handler = NULL
    }
  };

void
dacp_request(struct evhttp_request *req)
{
  char *full_uri;
  char *uri;
  char *ptr;
  char *uri_parts[7];
  struct evbuffer *evbuf;
  struct evkeyvalq query;
  int handler;
  int ret;
  int i;

  memset(&query, 0, sizeof(struct evkeyvalq));

  full_uri = httpd_fixup_uri(req);
  if (!full_uri)
    {
      evhttp_send_error(req, HTTP_BADREQUEST, "Bad Request");
      return;
    }

  ptr = strchr(full_uri, '?');
  if (ptr)
    *ptr = '\0';

  uri = strdup(full_uri);
  if (!uri)
    {
      free(full_uri);
      evhttp_send_error(req, HTTP_BADREQUEST, "Bad Request");
      return;
    }

  if (ptr)
    *ptr = '?';

  ptr = uri;
  uri = evhttp_decode_uri(uri);
  free(ptr);

  DPRINTF(E_DBG, L_DACP, "DACP request: %s\n", full_uri);

  handler = -1;
  for (i = 0; dacp_handlers[i].handler; i++)
    {
      ret = regexec(&dacp_handlers[i].preg, uri, 0, NULL, 0);
      if (ret == 0)
        {
          handler = i;
          break;
        }
    }

  if (handler < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Unrecognized DACP request\n");

      evhttp_send_error(req, HTTP_BADREQUEST, "Bad Request");

      free(uri);
      free(full_uri);
      return;
    }

  /* DACP has no HTTP authentication - Remote is identified by its pairing-guid */

  memset(uri_parts, 0, sizeof(uri_parts));

  uri_parts[0] = strtok_r(uri, "/", &ptr);
  for (i = 1; (i < sizeof(uri_parts) / sizeof(uri_parts[0])) && uri_parts[i - 1]; i++)
    {
      uri_parts[i] = strtok_r(NULL, "/", &ptr);
    }

  if (!uri_parts[0] || uri_parts[i - 1] || (i < 2))
    {
      DPRINTF(E_LOG, L_DACP, "DACP URI has too many/few components (%d)\n", (uri_parts[0]) ? i : 0);

      evhttp_send_error(req, HTTP_BADREQUEST, "Bad Request");

      free(uri);
      free(full_uri);
      return;
    }

  evbuf = evbuffer_new();
  if (!evbuf)
    {
      DPRINTF(E_LOG, L_DACP, "Could not allocate evbuffer for DACP reply\n");

      evhttp_send_error(req, HTTP_SERVUNAVAIL, "Internal Server Error");

      free(uri);
      free(full_uri);
      return;
    }

  evhttp_parse_query(full_uri, &query);

  evhttp_add_header(req->output_headers, "DAAP-Server", "forked-daapd/" VERSION);
  /* Content-Type for all DACP replies; can be overriden as needed */
  evhttp_add_header(req->output_headers, "Content-Type", "application/x-dmap-tagged");

  dacp_handlers[handler].handler(req, evbuf, uri_parts, &query);

  evbuffer_free(evbuf);
  evhttp_clear_headers(&query);
  free(uri);
  free(full_uri);
}

int
dacp_is_request(struct evhttp_request *req, char *uri)
{
  if (strncmp(uri, "/ctrl-int/", strlen("/ctrl-int/")) == 0)
    return 1;
  if (strcmp(uri, "/ctrl-int") == 0)
    return 1;

  return 0;
}


int
dacp_init(void)
{
  char buf[64];
  int i;
  int ret;

  current_rev = 2;
  update_requests = NULL;

#ifdef USE_EVENTFD
  update_efd = eventfd(0, EFD_CLOEXEC);
  if (update_efd < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Could not create update eventfd: %s\n", strerror(errno));

      return -1;
    }
#else
# if defined(__linux__)
  ret = pipe2(update_pipe, O_CLOEXEC);
# else
  ret = pipe(update_pipe);
# endif
  if (ret < 0)
    {
      DPRINTF(E_LOG, L_DACP, "Could not create update pipe: %s\n", strerror(errno));

      return -1;
    }
#endif /* USE_EVENTFD */

  for (i = 0; dacp_handlers[i].handler; i++)
    {
      ret = regcomp(&dacp_handlers[i].preg, dacp_handlers[i].regexp, REG_EXTENDED | REG_NOSUB);
      if (ret != 0)
        {
          regerror(ret, &dacp_handlers[i].preg, buf, sizeof(buf));

          DPRINTF(E_FATAL, L_DACP, "DACP init failed; regexp error: %s\n", buf);
	  goto regexp_fail;
        }
    }

#ifdef USE_EVENTFD
  event_set(&updateev, update_efd, EV_READ, playstatusupdate_cb, NULL);
#else
  event_set(&updateev, update_pipe[0], EV_READ, playstatusupdate_cb, NULL);
#endif
  event_base_set(evbase_httpd, &updateev);
  event_add(&updateev, NULL);

  player_set_update_handler(dacp_playstatus_update_handler);

  return 0;

 regexp_fail:
#ifdef USE_EVENTFD
  close(update_efd);
#else
  close(update_pipe[0]);
  close(update_pipe[1]);
#endif
  return -1;
}

void
dacp_deinit(void)
{
  struct dacp_update_request *ur;
  int i;

  player_set_update_handler(NULL);

  for (i = 0; dacp_handlers[i].handler; i++)
    regfree(&dacp_handlers[i].preg);

  for (ur = update_requests; update_requests; ur = update_requests)
    {
      update_requests = ur->next;

      if (ur->req->evcon)
	{
	  evhttp_connection_set_closecb(ur->req->evcon, NULL, NULL);
	  evhttp_connection_free(ur->req->evcon);
	}

      free(ur);
    }

  event_del(&updateev);

#ifdef USE_EVENTFD
  close(update_efd);
#else
  close(update_pipe[0]);
  close(update_pipe[1]);
#endif
}
