#! /bin/bash

MAKE_TOOL="make"
if [ "`uname -o`" == "Cygwin" ]; then
    MAKE_TOOL="nmake"
fi

mkdir -p ../_build
cd ../_build
cmake -DCMAKE_TOOLCHAIN_FILE=../acmake/toolchains/android.toolchain.cmake ..
$MAKE_TOOL $1
BUILD_OK=$?
cd -

if [ $BUILD_OK == "0" ] 
then
    echo "build done :-)"
else
    echo ":-( BUILD FAILED :-("
    exit 1
fi


