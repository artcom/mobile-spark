#! /bin/bash

#SPARK_COMPONENT_DIR=`pwd`/..

MYDIR="$( cd "$( dirname "$0" )" && pwd )"
SPARK_COMPONENT_DIR=$MYDIR/..

if [[ "`uname -s`" == *CYGWIN* ]]; then
    cd $(cygpath "$MOBILE_SPARK/")
else
    cd $MOBILE_SPARK
fi    

if [ -z "${ARM_TARGET:+1}" ]; then
    export ARM_TARGET="armeabi-v7a"
    echo "Using default ARM_TARGET $ARM_TARGET"
fi
if [ -z "${ANDROID_LEVEL:+1}" ]; then
    export ANDROID_LEVEL="9"
    echo "Using default ANDROID_LEVEL $ANDROID_LEVEL"
fi

SPARK_COMPONENT_NAME="TemplateApp" SPARK_COMPONENT_DIR=$SPARK_COMPONENT_DIR android/build_project.sh $*
cd $SPARK_COMPONENT_DIR/android
