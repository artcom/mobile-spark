#! /bin/bash

cd ../_iOS_build

export IS_TARGET_PLATFORM_IOS="TRUE"
export IOS_PLATFORM="iPhoneSimulator"

cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchains/ios.toolchain.Simulator.cmake ..
#$MAKE_TOOL

