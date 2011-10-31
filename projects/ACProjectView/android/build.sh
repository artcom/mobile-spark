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
PROJECT_NAME="ACProjectView" ../../../android/build_project.sh $*


