#! /bin/bash

# use the Distribution profile for codesigning
IDENTITY="iPhone Distribution"

MYDIR="$( cd "$( dirname "$0" )" && pwd )"

if [ -d "$MYDIR/build" ]; then
    rm -rf $MYDIR/build/*
else
    mkdir $MYDIR/build
fi

cd $MYDIR/build

cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=$MOBILE_SPARK/acmake/toolchains/ios.toolchain.iOS.cmake $MYDIR/..

xcodebuild -project $MYDIR/build/*.xcodeproj/ -target ALL_BUILD -configuration RELEASE CODE_SIGN_IDENTITY="${IDENTITY}"

BUILD_OK=$?

if [ $BUILD_OK == "0" ] 
then
    echo "build done :-)"
else
    echo ":-( BUILD FAILED :-("
    exit 1
fi

