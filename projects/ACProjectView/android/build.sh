#! /bin/bash

PROJECT_NAME="ACProjectView"

DEPLOY=0
for i in $*
do
    case $i in
        deploy)
            DEPLOY=1
            ;;
   esac
done

if [ $DEPLOY == "1" ]
then
    mkdir $PROJECT_NAME/assets
    cp -ra ../assets/* $PROJECT_NAME/assets
fi


SPARK_COMPONENT_DIR=`pwd`/..
if [[ "`uname -s`" == *CYGWIN* ]]; then
    cd $(cygpath "$MOBILE_SPARK/")
else
    cd $MOBILE_SPARK
fi    
SPARK_COMPONENT_NAME="ACProjectView" SPARK_COMPONENT_DIR=$SPARK_COMPONENT_DIR android/build_project.sh $*
cd $SPARK_COMPONENT_DIR/android
