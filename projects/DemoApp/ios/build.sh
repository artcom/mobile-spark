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

xcodebuild -project $MYDIR/build/*.xcodeproj/ -target ALL_BUILD -configuration Release CODE_SIGN_IDENTITY="${IDENTITY}"

BUILD_OK=$?

if [ $BUILD_OK == "0" ] 
then
    echo "build done :-)"

    #create .ipa bundle
    APP_NAME="$( ls $MYDIR/build/bin/ | grep '.app' | cut -d'.' -f1)"
    APP_BUNDLE=$APP_NAME.app

    echo "creating package '$MYDIR/build/$APP_NAME.ipa' ..."

    mkdir $MYDIR/build/bin/Payload
    mv $MYDIR/build/bin/$APP_BUNDLE $MYDIR/build/bin/Payload

    zip -r $MYDIR/build/$APP_NAME.ipa $MYDIR/build/bin/Payload >> /dev/null
    rm -rf $MYDIR/build/bin/Payload

    echo "done"

else
    echo ":-( BUILD FAILED :-("
    exit 1
fi



