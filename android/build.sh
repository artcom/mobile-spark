#! /bin/bash

MAKE_TOOL="make"
if [[ "`uname -s`" == *CYGWIN* ]]; then
    MAKE_TOOL="nmake"
fi

if [ -z "${ARM_TARGET:+1}" ]; then
    export ARM_TARGET="armeabi-v7a"
    echo "Using default ARM_TARGET $ARM_TARGET"
fi
if [ -z "${ANDROID_LEVEL:+1}" ]; then
    export ANDROID_LEVEL="9"
    echo "Using default ANDROID_LEVEL $ANDROID_LEVEL"
fi

mkdir -p ../_build
cd ../_build

cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchains/android.toolchain.cmake ..


$MAKE_TOOL $1
BUILD_OK=$?

# copy shared std libraries to libs
if [[ "`uname -s`" == *CYGWIN* ]]; then
    ANDROID_NDK_WIN=`cygpath -u "$ANDROID_NDK"`
    cp $ANDROID_NDK_WIN/sources/cxx-stl/gnu-libstdc++/libs/$ARM_TARGET/*.so lib/$ARM_TARGET/
else
    cp $ANDROID_NDK/sources/cxx-stl/gnu-libstdc++/libs/$ARM_TARGET/*.so lib/$ARM_TARGET/
fi

cd -

if [ $BUILD_OK == "0" ] 
then
    echo "build done :-)"
else
    echo ":-( BUILD FAILED :-("
    exit 1
fi


