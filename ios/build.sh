#! /bin/bash

#$MOBILE_SPARK

MYDIR="$( cd "$( dirname "$0" )" && pwd )"

if [ -d "$MYDIR/build" ]; then
    rm -rf $MYDIR/build/*
else
    mkdir $MYDIR/build
fi

cd $MYDIR/build

cmake -DCMAKE_TOOLCHAIN_FILE=$MOBILE_SPARK/acmake/toolchains/ios.toolchain.iOS.Xcode.cmake $MOBILE_SPARK
make $1
BUILD_OK=$?

if [ $BUILD_OK == "0" ] 
then
    echo "build done :-)"
else
    echo ":-( BUILD FAILED :-("
    exit 1
fi

