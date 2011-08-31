#!/bin/bash

echo "push /SparkViewer/assets/layouts/main.spark"
adb push ./SparkViewer/assets/layouts/main.spark /sdcard/

PUSHPATH="./dataToPush/*"

for f in $PUSHPATH
do
    echo "push $f"
    adb push $f /sdcard/
done

