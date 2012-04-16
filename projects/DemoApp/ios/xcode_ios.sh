#!/bin/bash

cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=$MOBILE_SPARK/acmake/toolchains/ios.toolchain.iOS.cmake ../..
