#! /bin/bash
cd SparkViewerTest

ANDROID_TOOL="android"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
fi

# build native
ndk-build 

# update android test project
$ANDROID_TOOL update test-project -m ../SparkViewer -p SparkViewerTest/

# build test apk && upload && run test
ant run-tests