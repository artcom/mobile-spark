#! /bin/bash

MAKE_TOOL="make"
if [[ "`uname -s`" == *CYGWIN* ]]; then
    MAKE_TOOL="nmake"
fi

mkdir -p ../_build
cd ../_build

cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchains/android.toolchain.cmake ..


$MAKE_TOOL $1
BUILD_OK=$?

# copy shared std libraries to libs
if [[ "`uname -s`" == *CYGWIN* ]]; then
    ANDROID_NDK_WIN=`cygpath -u "$ANDROID_NDK"`
    cp $ANDROID_NDK_WIN/sources/cxx-stl/gnu-libstdc++/libs/armeabi-v7a/*.so lib/armeabi-v7a/
else
    cp $ANDROID_NDK/sources/cxx-stl/gnu-libstdc++/libs/armeabi-v7a/*.so lib/armeabi-v7a/
fi

cd -

if [ $BUILD_OK == "0" ] 
then
    echo "build done :-)"
else
    echo ":-( BUILD FAILED :-("
    exit 1
fi


