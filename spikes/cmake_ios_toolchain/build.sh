#! /bin/bash

export TEST="jröglkadlkfjadsfjasöldfj"
rm -rf build
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake ..
#cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/android.toolchain.cmake ..
#$MAKE_TOOL

