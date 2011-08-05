#! /bin/bash

ANDROID_TOOL="android"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
fi

#cd SparkViewer
# build native
#ndk-build 

cd ../_build
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/android.toolchain.cmake ..
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/android.toolchain.cmake ..
make

cd -
cd SparkViewer

# update android project
$ANDROID_TOOL update project --name SparkViewer --path . 

# build java
ant compile

# build apk && upload
ant install

cd -
