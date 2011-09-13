#!/bin/bash

echo "push core/shaders"
PUSHPATH="../core/shaders/*"
for file in $PUSHPATH
do
    echo "push $file"
    adb push $file /sdcard/core/$file
done
