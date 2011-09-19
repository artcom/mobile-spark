#!/bin/bash

FOLDERS="models layouts shaders textures fonts" 
if [ "$#" != "0" ]; then
    FOLDERS=$*
fi

echo "push folders $FOLDERS"

for folder in $FOLDERS
do
    echo "push folder $folder"
    adb shell mkdir -p /sdcard/$PROJECT_NAME/$folder

    PUSHPATH="./"$folder"/*"
    for file in $PUSHPATH
    do
        echo "push $file"
        adb push $file /sdcard/$PROJECT_NAME/$file
    done
done


