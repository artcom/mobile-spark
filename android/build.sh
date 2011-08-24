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

cd -

cd SparkViewer

# update android project
$ANDROID_TOOL --silent update project --target android-9 --name SparkViewer --path . 

# build java
ant -quiet compile

# build apk && upload
ant -quiet install

cd -
