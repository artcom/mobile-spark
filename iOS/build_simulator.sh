#! /bin/bash

cd ../_iOS_build

cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchains/ios.toolchain.Simulator.cmake ..
#$MAKE_TOOL

