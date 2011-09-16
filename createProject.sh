#!/bin/bash

if [ "$#" = "0" ]; then
    echo "we need project name as first and only parameter"
    exit 0
fi

PROJECT_NAME=$1
echo "create new project $PROJECT_NAME" 

rm -rf projects/$PROJECT_NAME
cp -rf projects/TemplateApp projects/$PROJECT_NAME

cd projects/$PROJECT_NAME

cd android

sed s/Skeleton/$PROJECT_NAME/g "$SKELETON" > SCRIPTS/$PROJECT_NAME.js

