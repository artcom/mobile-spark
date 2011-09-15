#! /bin/bash

cd Xcode_Ipad

cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=../../acmake/toolchains/ios.toolchain.iOS.Xcode.cmake ..

cd -