#! /bin/bash

ANDROID_TOOL="android"
if [ "`uname -o`" == "Cygwin" ]; then
    ANDROID_TOOL="android.bat"
fi

# build native
#ndk-build 
cd ../_build
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/android.toolchain.cmake ..
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/android.toolchain.cmake ..
make

cd -


cd SparkViewerTest
# update android test project
$ANDROID_TOOL update test-project -m ../SparkViewer -p .

# build test apk && upload && run test
ant run-tests

cd -
