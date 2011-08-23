#! /bin/bash


export IS_TARGET_PLATFORM_IOS="TRUE"
export IOS_PLATFORM="iPhoneSimulator"
cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=../../acmake/toolchains/iOS.toolchain.Simulator.Xcode.cmake ../..
#$MAKE_TOOL