#!/bin/bash

SPARK_COMPONENT_DIR=`pwd`/..
#cd to mobile spark dir
cd ../../..
SPARK_COMPONENT_NAME="ACProjectView" SPARK_COMPONENT_DIR=$SPARK_COMPONENT_DIR android/push_project.sh $*
cd $SPARK_COMPONENT_DIR/android


