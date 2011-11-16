#! /bin/bash

NUMCORES=

#remove existing *.so files from common build-dir to avoid multiple project *.so-files there
rm -f $MOBILE_SPARK/_build/lib/armeabi-v7a/*

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

cd android
./build.sh $NUMCORES 
BUILD_OK=$?
echo "core build exited with $BUILD_OK"

cd $SPARK_COMPONENT_DIR

MAKE_TOOL="make"
if [ "`uname -o`" == "Cygwin" ]; then
    MAKE_TOOL="nmake"
fi

if [ $BUILD_OK == "0" ]
then
    mkdir -p _build
    cd _build
    cmake -DCMAKE_TOOLCHAIN_FILE=$MOBILE_SPARK/acmake/toolchains/android.toolchain.cmake ..
    $MAKE_TOOL $NUMCORES
    BUILD_OK=$?

    #copy projectname.so to core _build
    cd -
    cp _build/lib/armeabi-v7a/lib$SPARK_COMPONENT_NAME.so $MOBILE_SPARK/_build/lib/armeabi-v7a/
fi

exit $BUILD_OK

