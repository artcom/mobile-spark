#!/bin/bash

cd ..

PROJECT_NAME="ACProjectView" ../../android/push_project.sh $*

cd -
cd ../assets

PROJECT_NAME="ACProjectView"
for dir in ./*
do
    adb push $dir /sdcard/$PROJECT_NAME/$dir
done

cd -


