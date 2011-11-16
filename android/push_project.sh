#!/bin/bash

cd $SPARK_COMPONENT_DIR

FOLDERS="models layouts shaders textures fonts sounds" 
if [ "$#" != "0" ]; then
    FOLDERS=$*
fi

echo "push folders $FOLDERS"

for folder in $FOLDERS
do
    echo "push folder $folder in $SPARK_COMPONENT_NAME"
    adb shell mkdir -p /sdcard/$SPARK_COMPONENT_NAME/$folder

    PUSHPATH="./"$folder"/*"
    for file in $PUSHPATH
    do
        echo "push $file"
        adb push $file /sdcard/$SPARK_COMPONENT_NAME/$file
    done
done

cd $MOBILE_SPARK/core
PUSHPATH="shaders"
for dir in $PUSHPATH
do
    echo "push core/$dir"
    adb push $dir /sdcard/$SPARK_COMPONENT_NAME/$dir
done
cd -
