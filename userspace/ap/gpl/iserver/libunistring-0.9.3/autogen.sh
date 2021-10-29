#!/bin/sh
# Convenience script for regenerating all autogeneratable files that are
# omitted from the version control repository. In particular, this script
# also regenerates all aclocal.m4, config.h.in, Makefile.in, configure files
# with new versions of autoconf or automake.
#
# This script requires autoconf-2.63 and automake-1.11 in the PATH.
# It also requires either
#   - the GNULIB_TOOL environment variable pointing to the gnulib-tool script
#     in a gnulib checkout, or
#   - the git program in the PATH and an internet connection.
# It also requires
#   - the gperf program.

# Copyright (C) 2003-2009 Free Software Foundation, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Usage: ./autogen.sh [--skip-gnulib]
#
# Usage from a git checkout:                 ./autogen.sh
# This uses an up-to-date gnulib checkout.
#
# Usage from a released tarball:             ./autogen.sh --skip-gnulib
# This does not use a gnulib checkout.

skip_gnulib=false
while :; do
  case "$1" in
    --skip-gnulib) skip_gnulib=true; shift;;
    *) break ;;
  esac
done

if test $skip_gnulib = false; then
  # texinfo.tex
  # The most recent snapshot of it is available in the gnulib repository.
  # But this is a snapshot, with all possible dangers.
  # A stable release of it is available through "automake --add-missing --copy",
  # but that is too old (does not support @arrow{}). So take the version which
  # matches the latest stable texinfo release.
  if test ! -f build-aux/texinfo.tex; then
    { wget -q --timeout=5 -O build-aux/texinfo.tex.tmp 'http://cvs.savannah.gnu.org/viewvc/*checkout*/texinfo/doc/texinfo.tex?root=texinfo&pathrev=texinfo_4_13' \
        && mv build-aux/texinfo.tex.tmp build-aux/texinfo.tex; \
    } || rm -f build-aux/texinfo.tex.tmp
  fi
  if test -z "$GNULIB_TOOL"; then
    # Check out gnulib in a subdirectory 'gnulib'.
    if test -d gnulib; then
      (cd gnulib && git pull)
    else
      git clone git://git.savannah.gnu.org/gnulib.git
    fi
    # Now it should contain a gnulib-tool.
    if test -f gnulib/gnulib-tool; then
      GNULIB_TOOL=`pwd`/gnulib/gnulib-tool
    else
      echo "** warning: gnulib-tool not found" 1>&2
    fi
  fi
  # Skip the gnulib-tool step if gnulib-tool was not found.
  if test -n "$GNULIB_TOOL"; then
    if test -f m4/gnulib-cache.m4; then
      mv -f m4/gnulib-cache.m4 m4/gnulib-cache.m4~
    fi
    if test -f lib/Makefile.gnulib; then
      mv -f lib/Makefile.gnulib lib/Makefile.gnulib~
    fi
    GNULIB_MODULES='
      unitypes
      unistr/base
      unistr/u8-check
      unistr/u8-chr
      unistr/u8-cmp
      unistr/u8-cmp2
      unistr/u8-cpy
      unistr/u8-cpy-alloc
      unistr/u8-endswith
      unistr/u8-mblen
      unistr/u8-mbsnlen
      unistr/u8-mbtouc
      unistr/u8-mbtoucr
      unistr/u8-mbtouc-unsafe
      unistr/u8-move
      unistr/u8-next
      unistr/u8-prev
      unistr/u8-set
      unistr/u8-startswith
      unistr/u8-stpcpy
      unistr/u8-stpncpy
      unistr/u8-strcat
      unistr/u8-strchr
      unistr/u8-strcmp
      unistr/u8-strcoll
      unistr/u8-strcpy
      unistr/u8-strcspn
      unistr/u8-strdup
      unistr/u8-strlen
      unistr/u8-strmblen
      unistr/u8-strmbtouc
      unistr/u8-strncat
      unistr/u8-strncmp
      unistr/u8-strncpy
      unistr/u8-strnlen
      unistr/u8-strpbrk
      unistr/u8-strrchr
      unistr/u8-strspn
      unistr/u8-strstr
      unistr/u8-strtok
      unistr/u8-to-u16
      unistr/u8-to-u32
      unistr/u8-uctomb
      unistr/u16-check
      unistr/u16-chr
      unistr/u16-cmp
      unistr/u16-cmp2
      unistr/u16-cpy
      unistr/u16-cpy-alloc
      unistr/u16-endswith
      unistr/u16-mblen
      unistr/u16-mbsnlen
      unistr/u16-mbtouc
      unistr/u16-mbtoucr
      unistr/u16-mbtouc-unsafe
      unistr/u16-move
      unistr/u16-next
      unistr/u16-prev
      unistr/u16-set
      unistr/u16-startswith
      unistr/u16-stpcpy
      unistr/u16-stpncpy
      unistr/u16-strcat
      unistr/u16-strchr
      unistr/u16-strcmp
      unistr/u16-strcoll
      unistr/u16-strcpy
      unistr/u16-strcspn
      unistr/u16-strdup
      unistr/u16-strlen
      unistr/u16-strmblen
      unistr/u16-strmbtouc
      unistr/u16-strncat
      unistr/u16-strncmp
      unistr/u16-strncpy
      unistr/u16-strnlen
      unistr/u16-strpbrk
      unistr/u16-strrchr
      unistr/u16-strspn
      unistr/u16-strstr
      unistr/u16-strtok
      unistr/u16-to-u32
      unistr/u16-to-u8
      unistr/u16-uctomb
      unistr/u32-check
      unistr/u32-chr
      unistr/u32-cmp
      unistr/u32-cmp2
      unistr/u32-cpy
      unistr/u32-cpy-alloc
      unistr/u32-endswith
      unistr/u32-mblen
      unistr/u32-mbsnlen
      unistr/u32-mbtouc
      unistr/u32-mbtoucr
      unistr/u32-mbtouc-unsafe
      unistr/u32-move
      unistr/u32-next
      unistr/u32-prev
      unistr/u32-set
      unistr/u32-startswith
      unistr/u32-stpcpy
      unistr/u32-stpncpy
      unistr/u32-strcat
      unistr/u32-strchr
      unistr/u32-strcmp
      unistr/u32-strcoll
      unistr/u32-strcpy
      unistr/u32-strcspn
      unistr/u32-strdup
      unistr/u32-strlen
      unistr/u32-strmblen
      unistr/u32-strmbtouc
      unistr/u32-strncat
      unistr/u32-strncmp
      unistr/u32-strncpy
      unistr/u32-strnlen
      unistr/u32-strpbrk
      unistr/u32-strrchr
      unistr/u32-strspn
      unistr/u32-strstr
      unistr/u32-strtok
      unistr/u32-to-u16
      unistr/u32-to-u8
      unistr/u32-uctomb
      uniconv/base
      uniconv/u8-conv-from-enc
      uniconv/u8-conv-to-enc
      uniconv/u8-strconv-from-enc
      uniconv/u8-strconv-from-locale
      uniconv/u8-strconv-to-enc
      uniconv/u8-strconv-to-locale
      uniconv/u16-conv-from-enc
      uniconv/u16-conv-to-enc
      uniconv/u16-strconv-from-enc
      uniconv/u16-strconv-from-locale
      uniconv/u16-strconv-to-enc
      uniconv/u16-strconv-to-locale
      uniconv/u32-conv-from-enc
      uniconv/u32-conv-to-enc
      uniconv/u32-strconv-from-enc
      uniconv/u32-strconv-from-locale
      uniconv/u32-strconv-to-enc
      uniconv/u32-strconv-to-locale
      unistdio/base
      unistdio/u8-asnprintf
      unistdio/u8-asprintf
      unistdio/u8-snprintf
      unistdio/u8-sprintf
      unistdio/u8-u8-asnprintf
      unistdio/u8-u8-asprintf
      unistdio/u8-u8-snprintf
      unistdio/u8-u8-sprintf
      unistdio/u8-u8-vasnprintf
      unistdio/u8-u8-vasprintf
      unistdio/u8-u8-vsnprintf
      unistdio/u8-u8-vsprintf
      unistdio/u8-vasnprintf
      unistdio/u8-vasprintf
      unistdio/u8-vsnprintf
      unistdio/u8-vsprintf
      unistdio/u16-asnprintf
      unistdio/u16-asprintf
      unistdio/u16-snprintf
      unistdio/u16-sprintf
      unistdio/u16-u16-asnprintf
      unistdio/u16-u16-asprintf
      unistdio/u16-u16-snprintf
      unistdio/u16-u16-sprintf
      unistdio/u16-u16-vasnprintf
      unistdio/u16-u16-vasprintf
      unistdio/u16-u16-vsnprintf
      unistdio/u16-u16-vsprintf
      unistdio/u16-vasnprintf
      unistdio/u16-vasprintf
      unistdio/u16-vsnprintf
      unistdio/u16-vsprintf
      unistdio/u32-asnprintf
      unistdio/u32-asprintf
      unistdio/u32-snprintf
      unistdio/u32-sprintf
      unistdio/u32-u32-asnprintf
      unistdio/u32-u32-asprintf
      unistdio/u32-u32-snprintf
      unistdio/u32-u32-sprintf
      unistdio/u32-u32-vasnprintf
      unistdio/u32-u32-vasprintf
      unistdio/u32-u32-vsnprintf
      unistdio/u32-u32-vsprintf
      unistdio/u32-vasnprintf
      unistdio/u32-vasprintf
      unistdio/u32-vsnprintf
      unistdio/u32-vsprintf
      unistdio/ulc-asnprintf
      unistdio/ulc-asprintf
      unistdio/ulc-fprintf
      unistdio/ulc-snprintf
      unistdio/ulc-sprintf
      unistdio/ulc-vasnprintf
      unistdio/ulc-vasprintf
      unistdio/ulc-vfprintf
      unistdio/ulc-vsnprintf
      unistdio/ulc-vsprintf
      uniname/base
      uniname/uniname
      unictype/base
      unictype/bidicategory-all
      unictype/block-all
      unictype/category-all
      unictype/combining-class
      unictype/ctype-alnum
      unictype/ctype-alpha
      unictype/ctype-blank
      unictype/ctype-cntrl
      unictype/ctype-digit
      unictype/ctype-graph
      unictype/ctype-lower
      unictype/ctype-print
      unictype/ctype-punct
      unictype/ctype-space
      unictype/ctype-upper
      unictype/ctype-xdigit
      unictype/decimal-digit
      unictype/digit
      unictype/mirror
      unictype/numeric
      unictype/property-all
      unictype/scripts-all
      unictype/syntax-c-ident
      unictype/syntax-c-whitespace
      unictype/syntax-java-ident
      unictype/syntax-java-whitespace
      uniwidth/base
      uniwidth/u8-strwidth
      uniwidth/u8-width
      uniwidth/u16-strwidth
      uniwidth/u16-width
      uniwidth/u32-strwidth
      uniwidth/u32-width
      uniwidth/width
      uniwbrk/base
      uniwbrk/u8-wordbreaks
      uniwbrk/u16-wordbreaks
      uniwbrk/u32-wordbreaks
      uniwbrk/ulc-wordbreaks
      uniwbrk/wordbreak-property
      unilbrk/base
      unilbrk/u8-possible-linebreaks
      unilbrk/u8-width-linebreaks
      unilbrk/u16-possible-linebreaks
      unilbrk/u16-width-linebreaks
      unilbrk/u32-possible-linebreaks
      unilbrk/u32-width-linebreaks
      unilbrk/ulc-possible-linebreaks
      unilbrk/ulc-width-linebreaks
      uninorm/base
      uninorm/canonical-decomposition
      uninorm/composition
      uninorm/decomposition
      uninorm/filter
      uninorm/nfc
      uninorm/nfd
      uninorm/nfkc
      uninorm/nfkd
      uninorm/u8-normalize
      uninorm/u8-normcmp
      uninorm/u8-normcoll
      uninorm/u8-normxfrm
      uninorm/u16-normalize
      uninorm/u16-normcmp
      uninorm/u16-normcoll
      uninorm/u16-normxfrm
      uninorm/u32-normalize
      uninorm/u32-normcmp
      uninorm/u32-normcoll
      uninorm/u32-normxfrm
      unicase/base
      unicase/empty-prefix-context
      unicase/empty-suffix-context
      unicase/locale-language
      unicase/tolower
      unicase/totitle
      unicase/toupper
      unicase/u8-casecmp
      unicase/u8-casecoll
      unicase/u8-casefold
      unicase/u8-casexfrm
      unicase/u8-ct-casefold
      unicase/u8-ct-tolower
      unicase/u8-ct-totitle
      unicase/u8-ct-toupper
      unicase/u8-is-cased
      unicase/u8-is-casefolded
      unicase/u8-is-lowercase
      unicase/u8-is-titlecase
      unicase/u8-is-uppercase
      unicase/u8-tolower
      unicase/u8-totitle
      unicase/u8-toupper
      unicase/u16-casecmp
      unicase/u16-casecoll
      unicase/u16-casefold
      unicase/u16-casexfrm
      unicase/u16-ct-casefold
      unicase/u16-ct-tolower
      unicase/u16-ct-totitle
      unicase/u16-ct-toupper
      unicase/u16-is-cased
      unicase/u16-is-casefolded
      unicase/u16-is-lowercase
      unicase/u16-is-titlecase
      unicase/u16-is-uppercase
      unicase/u16-tolower
      unicase/u16-totitle
      unicase/u16-toupper
      unicase/u32-casecmp
      unicase/u32-casecoll
      unicase/u32-casefold
      unicase/u32-casexfrm
      unicase/u32-ct-casefold
      unicase/u32-ct-tolower
      unicase/u32-ct-totitle
      unicase/u32-ct-toupper
      unicase/u32-is-cased
      unicase/u32-is-casefolded
      unicase/u32-is-lowercase
      unicase/u32-is-titlecase
      unicase/u32-is-uppercase
      unicase/u32-tolower
      unicase/u32-totitle
      unicase/u32-toupper
      unicase/ulc-casecmp
      unicase/ulc-casecoll
      unicase/ulc-casexfrm
      relocatable-lib-lgpl
    '
    $GNULIB_TOOL --lib=libunistring --source-base=lib --m4-base=gnulib-m4 --tests-base=tests \
      --with-tests --lgpl --makefile-name=Makefile.gnulib --libtool --local-dir=gnulib-local \
      --import $GNULIB_MODULES
    # Change lib/unistr.h to be usable standalone.
    sed -e 's/ifdef GNULIB_[A-Za-z0-9_]*/if 1/' -e 's/defined GNULIB_[A-Za-z0-9_]*/1/g' \
        -e 's/HAVE_INLINE/UNISTRING_HAVE_INLINE/g' \
        < lib/unistr.h \
        > lib/unistr.h.tmp \
    && mv lib/unistr.h.tmp lib/unistr.h
    # Change lib/unictype.h and lib/uninorm.h for shared libraries on Woe32 systems.
    sed -e 's/extern const uc_general_category_t UC_/extern LIBUNISTRING_DLL_VARIABLE const uc_general_category_t UC_/' \
        -e 's/extern const uc_property_t UC_/extern LIBUNISTRING_DLL_VARIABLE const uc_property_t UC_/' \
        < lib/unictype.h \
        > lib/unictype.h.tmp \
    && mv lib/unictype.h.tmp lib/unictype.h
    sed -e 's/extern const struct unicode_normalization_form /extern LIBUNISTRING_DLL_VARIABLE const struct unicode_normalization_form /' \
        < lib/uninorm.h \
        > lib/uninorm.h.tmp \
    && mv lib/uninorm.h.tmp lib/uninorm.h
    sed -e 's/extern const casing_/extern LIBUNISTRING_DLL_VARIABLE const casing_/' \
        < lib/unicase.h \
        > lib/unicase.h.tmp \
    && mv lib/unicase.h.tmp lib/unicase.h
    $GNULIB_TOOL --copy-file build-aux/config.guess; chmod a+x build-aux/config.guess
    $GNULIB_TOOL --copy-file build-aux/config.sub;   chmod a+x build-aux/config.sub
    # If we got no texinfo.tex so far, take the snapshot from gnulib.
    if test ! -f build-aux/texinfo.tex; then
      $GNULIB_TOOL --copy-file build-aux/texinfo.tex build-aux/texinfo.tex
    fi
  fi
fi

build-aux/fixaclocal aclocal -I m4 -I gnulib-m4
autoconf
autoheader && touch config.h.in
automake --add-missing --copy
