#!/bin/sh
#
##################################################################################
#                                                                                 #
# Copyright (C) 2018, Broadcom. All Rights Reserved.                              #
#                                                                                 #
# Permission to use, copy, modify, and/or distribute this software for any        #
# purpose with or without fee is hereby granted, provided that the above          #
# copyright notice and this permission notice appear in all copies.               #
#                                                                                 #
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES        #
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF                #
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY     #
# SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES              #
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION    #
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN          #
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.                        #
#                                                                                 #
###################################################################################

SRC=$1
DEST=$2

#echo "source=$SRC destination=$DEST"

mkdir -p $DEST

# copy timezone list into destination folder

cp "$SRC/Etc/GMT-12"            "$DEST/International_Date_Line_West"
cp "$SRC/Pacific/Midway"        "$DEST/Midway_Island"
cp "$SRC/US/Hawaii"             "$DEST/Hawaii"
cp "$SRC/US/Alaska"             "$DEST/Alaska"
cp "$SRC/America/Tijuana"       "$DEST/Pacific_Time"
cp "$SRC/US/Arizona"            "$DEST/Arizona"
cp "$SRC/America/Chihuahua"     "$DEST/Chihuahua"
cp "$SRC/US/Mountain"           "$DEST/Mountain_Time"
cp "$SRC/US/Central"            "$DEST/Central_Time"
cp "$SRC/America/Monterrey"     "$DEST/Monterrey"
cp "$SRC/Canada/Saskatchewan"   "$DEST/Saskatchewan"
cp "$SRC/America/Bogota"        "$DEST/Bogota"
cp "$SRC/US/Eastern"            "$DEST/Eastern_Time"
cp "$SRC/America/Indianapolis"  "$DEST/Indiana"
cp "$SRC/Canada/Atlantic"       "$DEST/Atlantic_Time"
cp "$SRC/America/Caracas"       "$DEST/Caracas"
cp "$SRC/America/Santiago"      "$DEST/Santiago"
cp "$SRC/Canada/Newfoundland"   "$DEST/Newfoundland"
cp "$SRC/Brazil/East"           "$DEST/Brasilia"
cp "$SRC/America/Buenos_Aires"  "$DEST/Buenos_Aires"
cp "$SRC/America/Godthab"       "$DEST/Greenland"
cp "$SRC/America/Noronha"       "$DEST/Noronha"
cp "$SRC/Atlantic/Azores"       "$DEST/Azores"
cp "$SRC/Atlantic/Cape_Verde"   "$DEST/Cape_Verde_Is."
cp "$SRC/Africa/Casablanca"     "$DEST/Casablanca"
cp "$SRC/GMT"                   "$DEST/GMT"
cp "$SRC/Europe/Amsterdam"      "$DEST/Amsterdam"
cp "$SRC/Europe/Belgrade"       "$DEST/Belgrade"
cp "$SRC/Europe/Brussels"       "$DEST/Brussels"
cp "$SRC/Europe/Sarajevo"       "$DEST/Sarajevo"
cp "$SRC/Africa/Kinshasa"       "$DEST/Kinshasa"
cp "$SRC/Europe/Athens"         "$DEST/Athens"
cp "$SRC/Europe/Bucharest"      "$DEST/Bucharest"
cp "$SRC/Africa/Cairo"          "$DEST/Cairo"
cp "$SRC/Africa/Harare"         "$DEST/Harare"
cp "$SRC/Europe/Helsinki"       "$DEST/Helsinki"
cp "$SRC/Asia/Jerusalem"        "$DEST/Jerusalem"
cp "$SRC/Asia/Baghdad"          "$DEST/Baghdad"
cp "$SRC/Asia/Kuwait"           "$DEST/Kuwait"
cp "$SRC/Europe/Moscow"         "$DEST/Moscow"
cp "$SRC/Africa/Nairobi"        "$DEST/Nairobi"
cp "$SRC/Asia/Tehran"           "$DEST/Tehran"
cp "$SRC/Asia/Muscat"           "$DEST/Muscat"
cp "$SRC/Asia/Baku"             "$DEST/Baku"
cp "$SRC/Asia/Kabul"            "$DEST/Kabul"
cp "$SRC/Asia/Karachi"          "$DEST/Karachi"
cp "$SRC/Asia/Kolkata"          "$DEST/Kolkata"
cp "$SRC/Asia/Kathmandu"        "$DEST/Kathmandu"
cp "$SRC/Asia/Almaty"           "$DEST/Almaty"
cp "$SRC/Asia/Dhaka"            "$DEST/Dhaka"
cp "$SRC/Asia/Rangoon"          "$DEST/Rangoon"
cp "$SRC/Asia/Krasnoyarsk"      "$DEST/Krasnoyarsk"
cp "$SRC/Asia/Shanghai"         "$DEST/Beijing"
cp "$SRC/Asia/Irkutsk"          "$DEST/Irkutsk"
cp "$SRC/Asia/Kuala_Lumpur"     "$DEST/Kuala_Lumpur"
cp "$SRC/Australia/Perth"       "$DEST/Perth"
cp "$SRC/Asia/Taipei"           "$DEST/Taipei"
cp "$SRC/Asia/Tokyo"            "$DEST/Tokyo"
cp "$SRC/Asia/Seoul"            "$DEST/Seoul"
cp "$SRC/Asia/Yakutsk"          "$DEST/Yakutsk"
cp "$SRC/Australia/Adelaide"    "$DEST/Adelaide"
cp "$SRC/Australia/Darwin"      "$DEST/Darwin"
cp "$SRC/Australia/Brisbane"    "$DEST/Brisbane"
cp "$SRC/Australia/Canberra"    "$DEST/Canberra"
cp "$SRC/Pacific/Guam"          "$DEST/Guam"
cp "$SRC/Asia/Vladivostok"      "$DEST/Vladivostok"
cp "$SRC/Asia/Magadan"          "$DEST/Magadan"
cp "$SRC/Pacific/Auckland"      "$DEST/Auckland"
cp "$SRC/Pacific/Fiji"          "$DEST/Fiji"
