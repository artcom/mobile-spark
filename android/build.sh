#! /bin/bash

ANDROID_TOOL="android"
MAKE_TOOL="make"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
    MAKE_TOOL="nmake"
fi


cd ../_build
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchains/android.toolchain.cmake ..
$MAKE_TOOL $1
BUILD_OK=$?

cd -
echo "build done"
