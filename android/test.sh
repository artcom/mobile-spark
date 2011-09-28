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

# update Base project
$ANDROID_TOOL --silent update lib-project --target android-9 --path . 

cd -

cd SparkViewerTest
# update android test project
$ANDROID_TOOL update project --library ../SparkViewerBase --target android-9 --name test --path . 
$ANDROID_TOOL --silent update test-project --main ../SparkViewerBase --path .

# update test.spark
adb push assets/test.spark /sdcard/test.spark

# build test apk && upload && run test
ant  run-tests

cd -
