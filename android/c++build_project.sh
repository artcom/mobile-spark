#! /bin/bash

APPFOLDER=`pwd`/..
NUMCORES=

for i in $*
do
    case $i in
        -j*)
            NUMCORES=$i
            ;;
        *)
        #unknown
       ;;
   esac
done

cd ../../../android
./build.sh $NUMCORES 
BUILD_OK=$?
echo "core build exited with $BUILD_OK"

cd $APPFOLDER

MAKE_TOOL="make"
if [ "`uname -o`" == "Cygwin" ]; then
    MAKE_TOOL="nmake"
fi

if [ $BUILD_OK == "0" ]
then
    mkdir -p _build
    cd _build
    cmake -DCMAKE_TOOLCHAIN_FILE=../../acmake/toolchains/android.toolchain.cmake ..
    $MAKE_TOOL $NUMCORES
    BUILD_OK=$?

    #copy projectname.so to core _build
    cd -
    cp _build/lib/armeabi-v7a/lib$PROJECT_NAME.so ../../_build/lib/armeabi-v7a/
fi

exit $BUILD_OK

