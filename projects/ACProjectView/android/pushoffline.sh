#!/bin/bash

OLD_DIR=`pwd`

./push.sh

cd $OLD_DIR
cd ../assets

PROJECT_NAME="ACProjectView"
for dir in ./*
do
    adb push $dir /sdcard/$PROJECT_NAME/$dir
done

cd -


