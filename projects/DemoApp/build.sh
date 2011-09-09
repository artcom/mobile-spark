#! /bin/bash

APPFOLDER=`pwd`
cd ../../android
./build.sh
cd $APPFOLDER

ANDROID_TOOL="android"
MAKE_TOOL="make"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
    MAKE_TOOL="nmake"
fi

mkdir -p _build
cd _build
cmake -DCMAKE_TOOLCHAIN_FILE=../../acmake/toolchains/android.toolchain.cmake ..
$MAKE_TOOL $1
BUILD_OK=$?


#TODO: copy demoapp.so to _build
cd -


# package java
cd ../../android
if [ $BUILD_OK == "0" ] 
then
    cd SparkViewer
    
    # update android project
    $ANDROID_TOOL --silent update project --target android-9 --name SparkViewer --path . 
    BUILD_OK=$?
fi

if [ $BUILD_OK == "0" ] 
then
    
    # build java
    ant -quiet compile
    BUILD_OK=$?
fi
    
if [ $BUILD_OK == "0" ] 
then
    # build apk && upload
    ant -quiet install
    BUILD_OK=$?
fi
    
cd -

echo "build done"
