#! /bin/bash


export MACOSX_DEPLOYMENT_TARGET="Compiler Default"
cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=../../acmake/toolchains/iOS.toolchain.Simulator.Xcode.cmake ../..
#$MAKE_TOOL
