#! /bin/bash

APPFOLDER=`pwd`/..
echo $APPFOLDER
cd ../../../android
./build.sh $1
BUILD_OK=$?
echo "core build exited with $BUILD_OK"

cd $APPFOLDER

ANDROID_TOOL="android"
MAKE_TOOL="make"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
    MAKE_TOOL="nmake"
fi

if [ $BUILD_OK == "0" ]
then
    mkdir -p _build
    cd _build
    cmake -DCMAKE_TOOLCHAIN_FILE=../../acmake/toolchains/android.toolchain.cmake ..
    $MAKE_TOOL $1
    BUILD_OK=$?

    #copy demoapp.so to core _build
    cd -
    cp ../_build/lib/armeabi-v7a/libtemplateapp.so ../../../_build/lib/armeabi-v7a/
fi



# package java
cd android
if [ $BUILD_OK == "0" ] 
then
    cd TemplateActivity
    
    # update Base project
    $ANDROID_TOOL update lib-project --target android-9 --path ../../../../android/SparkViewerBase 

    # update android project
    $ANDROID_TOOL update project --target android-9 --name TemplateActivity --path . 
    $ANDROID_TOOL update project --library ../../../../android/SparkViewerBase --target android-9 --name TemplateActivity --path . 
    BUILD_OK=$?
fi

if [ $BUILD_OK == "0" ] 
then
    
    # build java
    ant -quiet   compile
    BUILD_OK=$?
fi
    
if [ $BUILD_OK == "0" ] 
then
    # build apk && upload
    ant -quiet install
    BUILD_OK=$?
fi
    
cd -


if [ $BUILD_OK == "0" ] 
then
    echo "build done :-)"
else
    echo ":-( BUILD FAILED :-("
    exit 1
fi
