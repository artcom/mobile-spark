#! /bin/bash

cd ../_iOS_build

export IS_TARGET_PLATFORM_IOS="TRUE"
export IOS_PLATFORM="iPhoneOS"

cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchains/ios.toolchain.cmake ..
#$MAKE_TOOL

