#! /bin/bash


export IS_TARGET_PLATFORM_IOS="TRUE"
export IOS_PLATFORM="iPhoneOS"
cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=../../acmake/toolchains/ios.toolchain.iOS.Xcode.cmake ../..
#$MAKE_TOOL