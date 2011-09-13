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

cd -


adb push SparkViewerTest/assets/test.spark /sdcard/test.spark

cd SparkViewerTest
# update android test project
$ANDROID_TOOL --silent update test-project -m ../SparkViewerBase -p .

# build test apk && upload && run test
ant  run-tests

cd -
