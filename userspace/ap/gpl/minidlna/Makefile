############################################################################################
#
#	Makefile For zlib
#
#	Author: Bob
#	Date: 2020.09.23
#	
#	Description:
#
#############################################################################################
include $(GPLTOPDIR)/config.mk
include $(GPLTOPDIR)/config.in

MINI_DLNA_PATH=$(shell pwd)

all: zlib libogg libexif sqlite3 libvorbis flac libid3tag ffmpeg jpeg
	make copy
	rm minidlna-1.1.5 -rf
	tar -xzvf minidlna-1.1.5.tar.gz
	cd minidlna-1.1.5 && (./configure --host=arm --enable-rpath --enable-tivo --enable-netgear \
	LDFLAGS="-lpthread ${LDFLAGS:-} -Wl,-rpath,../lib -L../lib" \
	CFLAGS="-g -O3 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -lpthread -I../ffmpeg-3.4.1/ -I../jpeg-7 \
			-I../ffmpeg-3.4.1/libavformat -I../libvorbis-1.2.3/include \
			-I../sqlite-3.6.22 -I../libexif-0.6.19 -I../libid3tag-0.15.0b \
			-I../libogg-1.1.4/include \
			-I../libvorbis-1.3.5/include/vorbis -I../flac-1.2.1/include");
	patch -p1 -i ./minidlna-1.1.5_2020_0923.patch
	make -C minidlna-1.1.5
	cp ./minidlna-1.1.5/minidlnad	./lib/minidlna.exe

zlib:
	cd zlib-1.2.8 && (./configure --sharedlibdir=../lib);
	cd zlib-1.2.8 && make

libogg:
	cd libogg-1.1.4 && (./configure --host=$(TARGET_PLATFORM_ARCH)-linux CC=$(CC));
	cd libogg-1.1.4 && make

libexif:
	cd libexif-0.6.19 && (./configure --host=$(TARGET_PLATFORM_ARCH)-linux CC=$(CC));
	cd libexif-0.6.19 && make

sqlite3:
	#rm sqlite-3.6.22 -rf
	tar -xzvf sqlite-3.6.22.tar.gz
	cd sqlite-3.6.22 && (./configure --disable-tcl --host=$(TARGET_PLATFORM_ARCH)-linux CC=$(CC));
	cd sqlite-3.6.22 && make

libvorbis:
	#rm libvorbis-1.2.3 -rf
	tar -xzvf libvorbis-1.2.3.tar.gz
	cd libvorbis-1.2.3 && (./configure --host=$(TARGET_PLATFORM_ARCH)-linux CC=$(CC) \
					CFLAGS=-I$(MINI_DLNA_PATH)/libogg-1.1.4/include/ \
					LDFLAGS="-L$(MINI_DLNA_PATH)/libogg-1.1.4/src/.libs/ -logg -lm");
	make -C libvorbis-1.2.3

flac:
	cd flac-1.2.1 && (./configure --host=$(TARGET_PLATFORM_ARCH)-linux CC=$(CC) \
				CFLAGS=-I$(MINI_DLNA_PATH)/libogg-1.1.4/include/ \
				LDFLAGS="-L$(MINI_DLNA_PATH)/libogg-1.1.4/src/.libs/ -logg");
	make -C flac-1.2.1
				
libid3tag:
	#rm libid3tag-0.15.0b -rf
	tar -xzvf libid3tag-0.15.0b.tar.gz
	cd libid3tag-0.15.0b && (./configure --host=$(TARGET_PLATFORM_ARCH)-linux CC=$(CC) \
				CPPFLAGS=-I$(MINI_DLNA_PATH)/zlib-1.2.8/ \
				LDFLAGS=-L$(MINI_DLNA_PATH)/zlib-1.2.8/ );
	cd libid3tag-0.15.0b && make

ffmpeg:
	cd ffmpeg-3.4.1 && (./configure --enable-shared --enable-gpl --arch=arm \
				--cc=$(CC) --enable-cross-compile --strip=$(STRIP) \
				--disable-muxers --disable-encoders --disable-filters --target_os=linux \
				--extra-cflags="-fPIC -lgcc_s" );
	cd ffmpeg-3.4.1 && make

jpeg:
	cd jpeg-7 && (./configure --host=$(TARGET_PLATFORM_ARCH)-linux CC=$(CC));
	cd jpeg-7 && make
					
copy:
	cp ./zlib-1.2.8/libz.so.1.2.8				./lib/libz.so.1 && \
	cp ./zlib-1.2.8/libz.so.1.2.8				./lib/libz.so && \
	cp ./libvorbis-1.2.3/lib/.libs/libvorbis.so.0.4.3	./lib/libvorbis.so.0 && \
	cp ./libvorbis-1.2.3/lib/.libs/libvorbis.so.0.4.3	./lib/libvorbis.so && \
	cp ./flac-1.2.1/src/libFLAC/.libs/libFLAC.so.8.2.0	./lib/libFLAC.so.8 && \
	cp ./flac-1.2.1/src/libFLAC/.libs/libFLAC.so.8.2.0	./lib/libFLAC.so && \
	cp ./libexif-0.6.19/libexif/.libs/libexif.so.12.3.1 	./lib/libexif.so.12  && \
	cp ./libexif-0.6.19/libexif/.libs/libexif.so.12.3.1 	./lib/libexif.so  && \
	cp ./libogg-1.1.4/src/.libs/libogg.so.0.6.0		./lib/libogg.so.0 && \
	cp ./libogg-1.1.4/src/.libs/libogg.so.0.6.0		./lib/libogg.so && \
	cp ./ffmpeg-3.4.1/libavutil/libavutil.so.55		./lib/libavutil.so.55 && \
	cp ./ffmpeg-3.4.1/libavdevice/libavdevice.so.57		./lib/libavdevice.so.57 && \
	cp ./ffmpeg-3.4.1/libavformat/libavformat.so.57		./lib/libavformat.so.57 && \
	cp ./ffmpeg-3.4.1/libavcodec/libavcodec.so.57		./lib/libavcodec.so.57 && \
	cp ./ffmpeg-3.4.1/libswresample/libswresample.so.2	./lib/libswresample.so.2 && \
	cp ./ffmpeg-3.4.1/libavutil/libavutil.so.55		./lib/libavutil.so && \
	cp ./ffmpeg-3.4.1/libavdevice/libavdevice.so.57		./lib/libavdevice.so && \
	cp ./ffmpeg-3.4.1/libavformat/libavformat.so.57		./lib/libavformat.so && \
	cp ./ffmpeg-3.4.1/libavcodec/libavcodec.so.57		./lib/libavcodec.so && \
	cp ./ffmpeg-3.4.1/libavfilter/libavfilter.so.6		./lib/libavfilter.so && \
	cp ./ffmpeg-3.4.1/libavfilter/libavfilter.so.6		./lib/libavfilter.so.6 && \
	cp ./sqlite-3.6.22/.libs/libsqlite3.so.0.8.6 		./lib/libsqlite3.so.0 && \
	cp ./sqlite-3.6.22/.libs/libsqlite3.so.0.8.6 		./lib/libsqlite3.so && \
	cp ./jpeg-7/.libs/libjpeg.so.7.0.0			./lib/libjpeg.so.7 && \
	cp ./jpeg-7/.libs/libjpeg.so.7.0.0			./lib/libjpeg.so && \
	cp ./libid3tag-0.15.0b/.libs/libid3tag.so.0.2.0 	./lib/libid3tag.so.0

clean:
	cd zlib-1.2.8 && make
	cd libogg-1.1.4 && make
	cd libexif-0.6.19 && make
	cd sqlite-3.6.22 && make
	cd libvorbis-1.2.3 && make clean
	cd flac-1.2.1 && make clean
	cd libid3tag-0.15.0b && make
	cd ffmpeg-3.4.1 && make
	cd jpeg-7 && make
	rm ./lib/libz.so.1
	rm ./lib/libvorbis.so.0
	rm ./lib/libFLAC.so.8
	rm ./lib/libogg.so.0
	rm ./lib/libexif.so.12
	rm ./lib/libavutil.so.55
	rm ./lib/libavdevice.so.57
	rm ./lib/libavformat.so.57
	rm ./lib/libswresample.so.2
	rm ./lib/libavcodec.so.57
	rm ./lib/libsqlite3.so.0
	rm ./lib/libjpeg.so.7
	rm ./lib/libid3tag.so.0
	rm ./lib/minidlna.exe
	rm -f ./lib/*.so
		
install:
	$(STRIP) ./lib/*
	install -m 755 minidlna-1.1.5/minidlna.conf $(TARGETDIR)/usr/
	install -m 755 lib/libz.so.1  $(TARGETDIR)/lib
	install -m 755 lib/libvorbis.so.0 $(TARGETDIR)/lib
	install -m 755 lib/libFLAC.so.8 $(TARGETDIR)/lib
	install -m 755 lib/libexif.so.12 $(TARGETDIR)/lib
	install -m 755 lib/libogg.so.0 $(TARGETDIR)/lib
	install -m 755 lib/libavutil.so.55 $(TARGETDIR)/lib
	install -m 755 lib/libavdevice.so.57 $(TARGETDIR)/lib
	install -m 755 lib/libavformat.so.57 $(TARGETDIR)/lib
	install -m 755 lib/libswresample.so.2 $(TARGETDIR)/lib
	install -m 755 lib/libavcodec.so.57 $(TARGETDIR)/lib
	install -m 755 lib/libavfilter.so.6 $(TARGETDIR)/lib
	install -m 755 lib/libsqlite3.so.0 $(TARGETDIR)/lib
	install -m 755 lib/libjpeg.so.7 $(TARGETDIR)/lib
	install -m 755 lib/libid3tag.so.0 $(TARGETDIR)/lib
	install -m 755 lib/minidlna.exe $(TARGETDIR)/usr/sbin/
	