#! /bin/bash


export IS_TARGET_PLATFORM_IOS="TRUE"
export IOS_PLATFORM="iPhoneOS"

export MACOSX_DEPLOYMENT_TARGET="Compiler Default"
cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=../../acmake/toolchains/ios.toolchain.iOS.Xcode.cmake ../..
#$MAKE_TOOL