#! /bin/bash


export IS_TARGET_PLATFORM_IOS="TRUE"
#export IOS_PLATFORM="iPhoneOS"
export IOS_PLATFORM="iPhoneSimulator"
export MACOSX_DEPLOYMENT_TARGET="Compiler Default"
cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=../../acmake/toolchain/iOS.toolchain.Xcode.cmake ../..
#$MAKE_TOOL