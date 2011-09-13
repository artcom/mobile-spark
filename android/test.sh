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

cd SparkViewerBase

# update android project
$ANDROID_TOOL --silent update project --target android-9 --name SparkViewerBase --path . 
BUILD_OK=$?

if [ $BUILD_OK == "0" ] 
then
    # build java
    ant -quiet compile
    BUILD_OK=$?
fi

cd -


cd SparkViewerTest
# update android test project
$ANDROID_TOOL --silent update test-project -m ../SparkViewerBase -p .

# build test apk && upload && run test
ant  run-tests

cd -
