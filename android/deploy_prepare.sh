#!/bin/bash

echo "deploy_prepare.sh: copy assets to folder that will be included in apk"
FOLDERS="models layouts shaders textures fonts sounds" 
for folder in $FOLDERS
do
    echo "copy folder $folder"
    cp -ra $folder $JAVA_PROJECT_DIR/assets
done

CORE_FOLDERS="shaders"
for core_folder in $CORE_FOLDERS
do
    echo "copy folder core/$core_folder"
    cp -ra $MOBILE_SPARK_DIR/core/$core_folder $JAVA_PROJECT_DIR/assets
done            

# remove test libs
rm -f $MOBILE_SPARK_DIR/_build/lib/armeabi-v7a/*test*.*
