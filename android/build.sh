#! /bin/bash
cd SparkViewer

ANDROID_TOOL="android"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
fi

# build native
ndk-build 

# update android project
$ANDROID_TOOL update project --name SparkViewer --path . 

# build java
ant compile

# build apk && upload
ant install

cd -