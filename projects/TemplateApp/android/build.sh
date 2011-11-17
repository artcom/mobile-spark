#! /bin/bash

SPARK_COMPONENT_DIR=`pwd`/..
if [[ "`uname -s`" == *CYGWIN* ]]; then
    cd $(cygpath "$MOBILE_SPARK/")
else
    cd $MOBILE_SPARK
fi    
SPARK_COMPONENT_NAME="TemplateApp" SPARK_COMPONENT_DIR=$SPARK_COMPONENT_DIR android/build_project.sh $*
cd $SPARK_COMPONENT_DIR/android
