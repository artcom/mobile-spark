#! /bin/bash

SPARK_COMPONENT_DIR=`pwd`/..
cd $MOBILE_SPARK
SPARK_COMPONENT_NAME="TemplateApp" SPARK_COMPONENT_DIR=$SPARK_COMPONENT_DIR android/c++build_project.sh $*
cd $SPARK_COMPONENT_DIR/android
