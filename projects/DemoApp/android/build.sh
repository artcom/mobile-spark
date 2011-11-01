#! /bin/bash

PROJECT_NAME="DemoApp"

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
    cp -ra ../models/textures/* $PROJECT_NAME/assets
fi
PROJECT_NAME="DemoApp"  ../../../android/build_project.sh $*
