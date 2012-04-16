#! /bin/bash

MYDIR="$( cd "$( dirname "$0" )" && pwd )"

if [ -d "$MYDIR/xcode" ]; then
    rm -rf $MYDIR/xcode/*
else
    mkdir $MYDIR/xcode
fi

cd $MYDIR/xcode

cmake -GXcode -DCMAKE_TOOLCHAIN_FILE=$MOBILE_SPARK/acmake/toolchains/ios.toolchain.iOS.cmake $MOBILE_SPARK

GENERATE_OK=$?

if [ $GENERATE_OK == "0" ] 
then
    echo "successfully created Xcode-project in '$MYDIR/xcode'"
else
    echo ":-( FAILED creating Xcode-project :-("
    exit 1
fi
