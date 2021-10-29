/***********************************************************************
 *
 * <:copyright-BRCM:2019:DUAL/GPL:standard
 * 
 *    Copyright (c) 2019 Broadcom 
 *    All Rights Reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation (the "GPL").
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * 
 * A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 * writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * :>
 *
 ************************************************************************/
#include "bcm_flashutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 200

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
#ifndef DESKTOP_LINUX
   char line[SIZE];
   int start, end;

   if (argc < 4)
   {
      printf("usage: %s partition (1 = first : 2 = second : 3 = boot : 4 = non-boot,\n", argv[0]);
      printf("       start block (-1 = beginning), end block (-1 = end of partition),\n");
      printf("       key (optional, leave blank to see all ident values in image\n");
      return(-1);
   }

   start = atoi(argv[2]);
   end = atoi(argv[3]);
   int ret = bcmFlash_getIdent(atoi(argv[1]), &start, &end, argv[4], line, SIZE);

   if (!ret)
      printf("key not found\n");
   else if (ret < 0)
      return(-1);
   else
      printf("\nkey size %d/%d found at block 0x%x:\n%.*s\n", ret, SIZE, start, ret, line);
#endif
   return(0);
}

