#!/bin/bash
NDK=d:/ndk/android-ndk-r9
SYSROOT=$NDK/platforms/android-9/arch-arm/
#TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64
TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt/windows

function build_one
{
#   ./configure \
    --prefix=$PREFIX \
    --enable-shared \
    --disable-static \
    --disable-doc \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffprobe \
    --disable-ffserver \
    --disable-avdevice \
    --disable-doc \
    --disable-symver \
    --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
    --target-os=linux \
    --arch=arm \
    --enable-cross-compile \
    --sysroot=$SYSROOT \
    --extra-cflags="-Os -fpic $ADDI_CFLAGS" \
    --extra-ldflags="$ADDI_LDFLAGS" \
    $ADDITIONAL_CONFIGURE_FLAG
    
#make clean
#make -j4
#make install

#mv  libavcodec/log2_tab.o libavcodec/log2_tab.o.bak
rm -f libavformat/log2_tab.o
rm -f libswresample/log2_tab.o
export CC="ccache $TOOLCHAIN/bin/arm-linux-androideabi-gcc"
$CC -lm -lz -shared --sysroot=$SYSROOT -Wl,--no-undefined -Wl,-z,noexecstack $EXTRA_LDFLAGS compat/*.o libavutil/*.o libavfilter/*.o libavutil/arm/*.o libavcodec/*.o libavcodec/arm/*.o libavformat/*.o libswresample/*.o libswresample/arm/*.o libswscale/*.o -o $PREFIX/libffmpeg.so

cp $PREFIX/libffmpeg.so $PREFIX/libffmpeg-debug.so
$TOOLCHAIN/bin/arm-linux-androideabi-strip --strip-unneeded $PREFIX/libffmpeg.so
}

CPU=arm
#CPU=arm-v7a
#PREFIX=$(pwd)/android/$CPU 
PREFIX=D:/work/workspace2/xplayer/jni/ffmpeg/android/$CPU 

ADDI_CFLAGS="-marm"
build_one