/***********************************************************************
 *
<:copyright-BRCM:2020:DUAL/GPL:standard

   Copyright (c) 2020 Broadcom 
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
 *
 ************************************************************************/

#include <string.h>
#include "cms_retcodes.h"
#include "cms_util.h"
#include "cms_vbuf.h"

CmsVbuf* cmsVbuf_new()
{
    CmsVbuf *s;
    s = (CmsVbuf *) cmsMem_alloc(sizeof(CmsVbuf), ALLOC_ZEROIZE);
    if (s != NULL) {
        s->data = cmsMem_alloc(CMS_VBUF_INITIAL_SIZE, ALLOC_ZEROIZE);
        if (s->data == NULL)
        {
            cmsMem_free(s);
            return NULL;
        }
        s->maxSize = CMS_VBUF_INITIAL_SIZE;
        s->size = 0;
        s->index = 0;
    }
    return s;
}

void cmsVbuf_destroy(CmsVbuf *s)
{
    if (s->data != NULL)
    {
        cmsMem_free(s->data);
    }
    cmsMem_free(s);
}

size_t cmsVbuf_getSize(CmsVbuf *s)
{
    return s->size;
}

static CmsRet cmsVbuf_reserveSpace(CmsVbuf *vbuf, UINT32 size)
{
    if (vbuf->size + size > vbuf->maxSize)
    {
        UINT32 newSize;
        void *new;

        newSize = vbuf->maxSize + ((size > CMS_VBUF_GROW_SIZE) ?
                                    size : CMS_VBUF_GROW_SIZE);
        new = cmsMem_realloc(vbuf->data, newSize);
        if (new == NULL)
        {
            return CMSRET_RESOURCE_EXCEEDED;
        }
        vbuf->data = new;
        vbuf->maxSize = newSize;
    }
    return CMSRET_SUCCESS;
}

CmsRet cmsVbuf_put(CmsVbuf *vbuf, const void *data, size_t size)
{
    CmsRet ret;
    ret = cmsVbuf_reserveSpace(vbuf, size);
    if (ret == CMSRET_SUCCESS)
    {
        memcpy((char *)(vbuf->data) + vbuf->size, data, size);
        vbuf->size += size;
    }
    return ret;
}

CmsRet cmsVbuf_putUINT16(CmsVbuf *vbuf, UINT16 data)
{
    CmsRet ret;
    ret = cmsVbuf_put(vbuf, &data, sizeof(UINT16));
    return ret;
}

CmsRet cmsVbuf_putUINT32(CmsVbuf *vbuf, UINT32 data)
{
    CmsRet ret;
    ret = cmsVbuf_put(vbuf, &data, sizeof(UINT32));
    return ret;
}

CmsRet cmsVbuf_putUBOOL8(CmsVbuf *vbuf, UBOOL8 data)
{
    CmsRet ret;
    ret = cmsVbuf_put(vbuf, &data, sizeof(UBOOL8));
    return ret;
}

CmsRet cmsVbuf_putString(CmsVbuf *vbuf, const char* data)
{
    CmsRet ret;
    ret = cmsVbuf_put(vbuf, data, strlen(data) + 1);
    return ret;
}

void cmsVbuf_resetIndex(CmsVbuf *vbuf)
{
    vbuf->index = 0;
}

CmsRet cmsVbuf_get(CmsVbuf *vbuf, void *pData, size_t size)
{
    memcpy(pData, (char *)(vbuf->data) + vbuf->index, size);
    vbuf->index += size;
    return CMSRET_SUCCESS;
}

CmsRet cmsVbuf_getUINT16(CmsVbuf *vbuf, UINT16 *pData)
{
    CmsRet ret;
    ret = cmsVbuf_get(vbuf, pData, sizeof(UINT16));
    return ret;
}

CmsRet cmsVbuf_getUINT32(CmsVbuf *vbuf, UINT32 *pData)
{
    CmsRet ret;
    ret = cmsVbuf_get(vbuf, pData, sizeof(UINT32));
    return ret;
}

CmsRet cmsVbuf_getUBOOL8(CmsVbuf *vbuf, UBOOL8 *pData)
{
    CmsRet ret;
    ret = cmsVbuf_get(vbuf, pData, sizeof(UBOOL8));
    return ret;
}

CmsRet cmsVbuf_getString(CmsVbuf *vbuf, char **data)
{
    CmsRet ret = CMSRET_SUCCESS;
    char *s = cmsMem_strdup((char *)(vbuf->data) + vbuf->index);
    if (s == NULL)
    {
        cmsLog_error("Failed to duplicate string from index %d!", vbuf->index);
        return CMSRET_RESOURCE_EXCEEDED;
    }
    vbuf->index += (strlen(s) + 1);
    *data = s;
    cmsLog_debug("duplicated string from index %d:%s", vbuf->index, s);
    return ret;
}
