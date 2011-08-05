#! /bin/bash

ANDROID_TOOL="android"
MAKE_TOOL="make"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
    MAKE_TOOL="nmake"
fi

cd ../_build
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/android.toolchain.cmake ..
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/android.toolchain.cmake ..
$MAKE_TOOL

cd -


cd SparkViewerTest
# update android test project
$ANDROID_TOOL update test-project -m ../SparkViewer -p .

# build test apk && upload && run test
ant run-tests

cd -
