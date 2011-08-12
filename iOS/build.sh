#! /bin/bash

cd ../_iOS_build

export IS_TARGET_PLATFORM_IOS="TRUE"
#export IOS_PLATFORM="iPhoneOS"
export IOS_PLATFORM="iPhoneSimulator"

cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/iOS.toolchain.cmake ..
#cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchain/android.toolchain.cmake ..
#$MAKE_TOOL

