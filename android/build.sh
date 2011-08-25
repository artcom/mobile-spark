#! /bin/bash

ANDROID_TOOL="android"
MAKE_TOOL="make"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
    MAKE_TOOL="nmake"
fi


cd ../_build
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchains/android.toolchain.cmake ..
$MAKE_TOOL
BUILD_OK=$?

cd -

if [ $BUILD_OK == "0" ] 
then
    cd SparkViewer
    
    # update android project
    $ANDROID_TOOL update project --target android-9 --name SparkViewer --path . 
    BUILD_OK=$?
fi

if [ $BUILD_OK == "0" ] 
then
    
    # build java
    ant compile
    BUILD_OK=$?
fi
    
if [ $BUILD_OK == "0" ] 
then
    # build apk && upload
    ant install
    BUILD_OK=$?
fi
    
cd -

echo "build done"