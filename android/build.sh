#! /bin/bash

ANDROID_TOOL="android"
MAKE_TOOL="make"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
    MAKE_TOOL="nmake"
fi

cd ../_build
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/android.toolchain.cmake ..
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/android.toolchain.cmake ..
$MAKE_TOOL

cd -

echo "push layout to sdcard"
adb push ../test.spark /sdcard/test.spark

cd SparkViewer

# update android project
$ANDROID_TOOL update project --target android-9 --name SparkViewer --path . 

# build java
ant compile

# build apk && upload
ant install

cd -
