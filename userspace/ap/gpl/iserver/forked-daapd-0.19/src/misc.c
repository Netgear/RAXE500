/*
 * Copyright (C) 2009-2010 Julien BLACHE <jb@jblache.org>
 *
 * Some code included below is in the public domain, check comments
 * in the file.
 *
 * Pieces of code adapted from mt-daapd:
 * Copyright (C) 2003-2007 Ron Pedde (ron@pedde.com)
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

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>
#include <sys/param.h>

#include <unistr.h>
#include <uniconv.h>

#include "logger.h"
#include "misc.h"


int
safe_atoi32(const char *str, int32_t *val)
{
  char *end;
  long intval;

  errno = 0;
  intval = strtol(str, &end, 10);

  if (((errno == ERANGE) && ((intval == LONG_MAX) || (intval == LONG_MIN)))
      || ((errno != 0) && (intval == 0)))
    {
      DPRINTF(E_DBG, L_MISC, "Invalid integer in string (%s): %s\n", str, strerror(errno));

      return -1;
    }

  if (end == str)
    {
      DPRINTF(E_DBG, L_MISC, "No integer found in string (%s)\n", str);

      return -1;
    }

  if (intval > INT32_MAX)
    {
      DPRINTF(E_DBG, L_MISC, "Integer value too large (%s)\n", str);

      return -1;
    }

  *val = (int32_t)intval;

  return 0;
}

int
safe_atou32(const char *str, uint32_t *val)
{
  char *end;
  unsigned long intval;

  errno = 0;
  intval = strtoul(str, &end, 10);

  if (((errno == ERANGE) && (intval == ULONG_MAX))
      || ((errno != 0) && (intval == 0)))
    {
      DPRINTF(E_DBG, L_MISC, "Invalid integer in string (%s): %s\n", str, strerror(errno));

      return -1;
    }

  if (end == str)
    {
      DPRINTF(E_DBG, L_MISC, "No integer found in string (%s)\n", str);

      return -1;
    }

  if (intval > UINT32_MAX)
    {
      DPRINTF(E_DBG, L_MISC, "Integer value too large (%s)\n", str);

      return -1;
    }

  *val = (uint32_t)intval;

  return 0;
}

int
safe_hextou32(const char *str, uint32_t *val)
{
  char *end;
  unsigned long intval;

  /* A hex shall begin with 0x */
  if (strncmp(str, "0x", 2) != 0)
    return safe_atou32(str, val);

  errno = 0;
  intval = strtoul(str, &end, 16);

  if (((errno == ERANGE) && (intval == ULONG_MAX))
      || ((errno != 0) && (intval == 0)))
    {
      DPRINTF(E_DBG, L_MISC, "Invalid integer in string (%s): %s\n", str, strerror(errno));

      return -1;
    }

  if (end == str)
    {
      DPRINTF(E_DBG, L_MISC, "No integer found in string (%s)\n", str);

      return -1;
    }

  if (intval > UINT32_MAX)
    {
      DPRINTF(E_DBG, L_MISC, "Integer value too large (%s)\n", str);

      return -1;
    }

  *val = (uint32_t)intval;

  return 0;
}

int
safe_atoi64(const char *str, int64_t *val)
{
  char *end;
  long long intval;

  errno = 0;
  intval = strtoll(str, &end, 10);

  if (((errno == ERANGE) && ((intval == LLONG_MAX) || (intval == LLONG_MIN)))
      || ((errno != 0) && (intval == 0)))
    {
      DPRINTF(E_DBG, L_MISC, "Invalid integer in string (%s): %s\n", str, strerror(errno));

      return -1;
    }

  if (end == str)
    {
      DPRINTF(E_DBG, L_MISC, "No integer found in string (%s)\n", str);

      return -1;
    }

  if (intval > INT64_MAX)
    {
      DPRINTF(E_DBG, L_MISC, "Integer value too large (%s)\n", str);

      return -1;
    }

  *val = (int64_t)intval;

  return 0;
}

int
safe_atou64(const char *str, uint64_t *val)
{
  char *end;
  unsigned long long intval;

  errno = 0;
  intval = strtoull(str, &end, 10);

  if (((errno == ERANGE) && (intval == ULLONG_MAX))
      || ((errno != 0) && (intval == 0)))
    {
      DPRINTF(E_DBG, L_MISC, "Invalid integer in string (%s): %s\n", str, strerror(errno));

      return -1;
    }

  if (end == str)
    {
      DPRINTF(E_DBG, L_MISC, "No integer found in string (%s)\n", str);

      return -1;
    }

  if (intval > UINT64_MAX)
    {
      DPRINTF(E_DBG, L_MISC, "Integer value too large (%s)\n", str);

      return -1;
    }

  *val = (uint64_t)intval;

  return 0;
}

int
safe_hextou64(const char *str, uint64_t *val)
{
  char *end;
  unsigned long long intval;

  errno = 0;
  intval = strtoull(str, &end, 16);

  if (((errno == ERANGE) && (intval == ULLONG_MAX))
      || ((errno != 0) && (intval == 0)))
    {
      DPRINTF(E_DBG, L_MISC, "Invalid integer in string (%s): %s\n", str, strerror(errno));

      return -1;
    }

  if (end == str)
    {
      DPRINTF(E_DBG, L_MISC, "No integer found in string (%s)\n", str);

      return -1;
    }

  if (intval > UINT64_MAX)
    {
      DPRINTF(E_DBG, L_MISC, "Integer value too large (%s)\n", str);

      return -1;
    }

  *val = (uint64_t)intval;

  return 0;
}


/* Key/value functions */
int
keyval_add_size(struct keyval *kv, const char *name, const char *value, size_t size)
{
  struct onekeyval *okv;
  const char *val;

  /* Check for duplicate key names */
  val = keyval_get(kv, name);
  if (val)
    {
      /* Same value, fine */
      if (strcmp(val, value) == 0)
        return 0;
      else /* Different value, bad */
        return -1;
    }

  okv = (struct onekeyval *)malloc(sizeof(struct onekeyval));
  if (!okv)
    {
      DPRINTF(E_LOG, L_MISC, "Out of memory for new keyval\n");

      return -1;
    }

  okv->name = strdup(name);
  if (!okv->name)
    {
      DPRINTF(E_LOG, L_MISC, "Out of memory for new keyval name\n");

      free(okv);
      return -1;
    }

  okv->value = (char *)malloc(size + 1);
  if (!okv->value)
    {
      DPRINTF(E_LOG, L_MISC, "Out of memory for new keyval value\n");

      free(okv->name);
      free(okv);
      return -1;
    }

  memcpy(okv->value, value, size);
  okv->value[size] = '\0';

  okv->next = NULL;

  if (!kv->head)
    kv->head = okv;

  if (kv->tail)
    kv->tail->next = okv;

  kv->tail = okv;

  return 0;
}

int
keyval_add(struct keyval *kv, const char *name, const char *value)
{
  return keyval_add_size(kv, name, value, strlen(value));
}

void
keyval_remove(struct keyval *kv, const char *name)
{
  struct onekeyval *okv;
  struct onekeyval *pokv;

  for (pokv = NULL, okv = kv->head; okv; pokv = okv, okv = okv->next)
    {
      if (strcasecmp(okv->name, name) == 0)
        break;
    }

  if (!okv)
    return;

  if (okv == kv->head)
    kv->head = okv->next;

  if (okv == kv->tail)
    kv->tail = pokv;

  if (pokv)
    pokv->next = okv->next;

  free(okv->name);
  free(okv->value);
  free(okv);
}

const char *
keyval_get(struct keyval *kv, const char *name)
{
  struct onekeyval *okv;

  for (okv = kv->head; okv; okv = okv->next)
    {
      if (strcasecmp(okv->name, name) == 0)
        return okv->value;
    }

  return NULL;
}

void
keyval_clear(struct keyval *kv)
{
  struct onekeyval *hokv;
  struct onekeyval *okv;

  hokv = kv->head;

  for (okv = hokv; hokv; okv = hokv)
    {
      hokv = okv->next;

      free(okv->name);
      free(okv->value);
      free(okv);
    }

  kv->head = NULL;
  kv->tail = NULL;
}


char *
m_realpath(const char *pathname)
{
  char buf[PATH_MAX];
  char *ret;

  ret = realpath(pathname, buf);
  if (!ret)
    return NULL;

  ret = strdup(buf);
  if (!ret)
    {
      DPRINTF(E_LOG, L_MISC, "Out of memory for realpath\n");

      return NULL;
    }

  return ret;
}

char *
unicode_fixup_string(char *str)
{
  uint8_t *ret;
  size_t len;

  if (!str)
    return NULL;

  len = strlen(str);

  /* String is valid UTF-8 */
  if (!u8_check((uint8_t *)str, len))
    {
      if (len >= 3)
	{
	  /* Check for and strip byte-order mark */
	  if (memcmp("\xef\xbb\xbf", str, 3) == 0)
	    memmove(str, str + 3, len - 3 + 1);
	}

      return str;
    }

  ret = u8_conv_from_encoding("ascii", iconveh_question_mark, str, len, NULL, NULL, &len);
  if (!ret)
    {
      DPRINTF(E_LOG, L_MISC, "Could not convert string '%s' to UTF-8: %s\n", str, strerror(errno));

      return NULL;
    }

  return (char *)ret;
}

uint32_t
djb_hash(void *data, size_t len)
{
  unsigned char *bytes = data;
  uint32_t hash = 5381;

  while (len--)
    {
      hash = ((hash << 5) + hash) + *bytes;
      bytes++;
    }

  return hash;
}


static unsigned char b64_decode_table[256];

char *
b64_decode(const char *b64)
{
  char *str;
  const unsigned char *iptr;
  unsigned char *optr;
  unsigned char c;
  int len;
  int i;

  if (b64_decode_table[0] == 0)
    {
      memset(b64_decode_table, 0xff, sizeof(b64_decode_table));

      /* Base64 encoding: A-Za-z0-9+/ */
      for (i = 0; i < 26; i++)
	{
	  b64_decode_table['A' + i] = i;
	  b64_decode_table['a' + i] = i + 26;
	}

      for (i = 0; i < 10; i++)
	b64_decode_table['0' + i] = i + 52;

      b64_decode_table['+'] = 62;
      b64_decode_table['/'] = 63;

      /* Stop on '=' */
      b64_decode_table['='] = 100; /* > 63 */
    }

  len = strlen(b64);

  str = (char *)malloc(len);
  if (!str)
    return NULL;

  memset(str, 0, len);

  iptr = (const unsigned char *)b64;
  optr = (unsigned char *)str;
  i = 0;

  while (len)
    {
      if (*iptr == '=')
	break;

      c = b64_decode_table[*iptr];
      if (c > 63)
	{
	  iptr++;
	  len--;
	  continue;
	}

      switch (i)
	{
	  case 0:
	    optr[0] = c << 2;
	    break;
	  case 1:
	    optr[0] |= c >> 4;
	    optr[1] = c << 4;
	    break;
	  case 2:
	    optr[1] |= c >> 2;
	    optr[2] = c << 6;
	    break;
	  case 3:
	    optr[2] |= c;
	    break;
	}

      i++;
      if (i == 4)
	{
	  optr += 3;
	  i = 0;
	}

      len--;
      iptr++;
    }

  return str;
}

static const char b64_encode_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void
b64_encode_block(uint8_t *in, char *out, int len)
{
  out[0] = b64_encode_table[in[0] >> 2];

  out[2] = out[3] = '=';

  if (len == 1)
    out[1] = b64_encode_table[((in[0] & 0x03) << 4)];
  else
    {
      out[1] = b64_encode_table[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];

      if (len == 2)
	out[2] = b64_encode_table[((in[1] & 0x0f) << 2)];
      else
	{
	  out[2] = b64_encode_table[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)];
	  out[3] = b64_encode_table[in[2] & 0x3f];
	}
    }
}

static void
b64_encode_full_block(uint8_t *in, char *out)
{
  out[0] = b64_encode_table[in[0] >> 2];
  out[1] = b64_encode_table[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
  out[2] = b64_encode_table[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)];
  out[3] = b64_encode_table[in[2] & 0x3f];
}

char *
b64_encode(uint8_t *in, size_t len)
{
  char *encoded;
  char *out;

  /* 3 in chars -> 4 out chars */
  encoded = (char *)malloc(len + (len / 3) + 4 + 1);
  if (!encoded)
    return NULL;

  out = encoded;

  while (len >= 3)
    {
      b64_encode_full_block(in, out);

      len -= 3;
      in += 3;
      out += 4;
    }

  if (len > 0)
    {
      b64_encode_block(in, out, len);
      out += 4;
    }

  out[0] = '\0';

  return encoded;
}

/*
 * MurmurHash2, 64-bit versions, by Austin Appleby
 *
 * Code released under the public domain, as per
 *    <http://murmurhash.googlepages.com/>
 * as of 2010-01-03.
 */

#if SIZEOF_VOID_P == 8 /* 64bit platforms */

uint64_t
murmur_hash64(const void *key, int len, uint32_t seed)
{
  const int r = 47;
  const uint64_t m = 0xc6a4a7935bd1e995;

  const uint64_t *data;
  const uint64_t *end;
  const unsigned char *data_tail;
  uint64_t h;
  uint64_t k;

  h = seed ^ (len * m);
  data = (const uint64_t *)key;
  end = data + (len / 8);

  while (data != end)
    {
      k = *data++;

      k *= m;
      k ^= k >> r;
      k *= m;

      h ^= k;
      h *= m;
    }

  data_tail = (const unsigned char *)data;

  switch (len & 7)
    {
      case 7:
	h ^= (uint64_t)(data_tail[6]) << 48;
      case 6:
	h ^= (uint64_t)(data_tail[5]) << 40;
      case 5:
	h ^= (uint64_t)(data_tail[4]) << 32;
      case 4:
	h ^= (uint64_t)(data_tail[3]) << 24;
      case 3:
	h ^= (uint64_t)(data_tail[2]) << 16;
      case 2:
	h ^= (uint64_t)(data_tail[1]) << 8;
      case 1:
	h ^= (uint64_t)(data_tail[0]);
	h *= m;
    }

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

#elif SIZEOF_VOID_P == 4 /* 32bit platforms */

uint64_t
murmur_hash64(const void *key, int len, uint32_t seed)
{
  const int r = 24;
  const uint32_t m = 0x5bd1e995;

  const uint32_t *data;
  const unsigned char *data_tail;
  uint32_t k1;
  uint32_t h1;
  uint32_t k2;
  uint32_t h2;

  uint64_t h;

  h1 = seed ^ len;
  h2 = 0;

  data = (const uint32_t *)key;

  while (len >= 8)
    {
      k1 = *data++;
      k1 *= m; k1 ^= k1 >> r; k1 *= m;
      h1 *= m; h1 ^= k1;

      k2 = *data++;
      k2 *= m; k2 ^= k2 >> r; k2 *= m;
      h2 *= m; h2 ^= k2;

      len -= 8;
    }

  if (len >= 4)
    {
      k1 = *data++;
      k1 *= m; k1 ^= k1 >> r; k1 *= m;
      h1 *= m; h1 ^= k1;
      len -= 4;
    }

  data_tail = (const unsigned char *)data;

  switch(len)
    {
      case 3:
	h2 ^= (uint32_t)(data_tail[2]) << 16;
      case 2:
	h2 ^= (uint32_t)(data_tail[1]) << 8;
      case 1:
	h2 ^= (uint32_t)(data_tail[0]);
	h2 *= m;
    };

  h1 ^= h2 >> 18; h1 *= m;
  h2 ^= h1 >> 22; h2 *= m;
  h1 ^= h2 >> 17; h1 *= m;
  h2 ^= h1 >> 19; h2 *= m;

  h = h1;
  h = (h << 32) | h2;

  return h;
}

#else
# error Platform not supported
#endif
