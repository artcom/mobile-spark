#! /bin/bash

ANDROID_TOOL="android"
MAKE_TOOL="make"
if [[ "`uname -s`" == *CYGWIN* ]]; then
    ANDROID_TOOL="android.bat"
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

cd ../_build
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchains/android.toolchain.cmake ..
$MAKE_TOOL $1
BUILD_OK=$?

if [ $BUILD_OK == "0" ] 
then
    echo "build done :-)"
else
    echo ":-( BUILD FAILED :-("
    exit 1
fi

# copy shared std libraries to libs
if [[ "`uname -s`" == *CYGWIN* ]]; then
    ANDROID_NDK_WIN=`cygpath -u "$ANDROID_NDK"`
    cp $ANDROID_NDK_WIN/sources/cxx-stl/gnu-libstdc++/libs/$ARM_TARGET/*.so lib/$ARM_TARGET/
else
    cp $ANDROID_NDK/sources/cxx-stl/gnu-libstdc++/libs/$ARM_TARGET/*.so lib/$ARM_TARGET/
fi

cd -

cd SparkViewerBase

# update Base project
$ANDROID_TOOL --silent update lib-project --target android-$ANDROID_LEVEL --path . 

cd -

cd SparkViewerTest
# update android test project
$ANDROID_TOOL update project --library ../SparkViewerBase --target android-$ANDROID_LEVEL --name test --path . 
$ANDROID_TOOL --silent update test-project --main ../SparkViewerBase --path .

# update test.spark
adb push assets/test.spark /sdcard/test.spark

# build test apk && upload && run test
ant -Dnative.libs.absolute.dir="$MOBILE_SPARK/_build/lib" debug
ant installt
ant test

cd -
