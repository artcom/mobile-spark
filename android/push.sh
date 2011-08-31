#!/bin/bash

echo -e "push data to sdcard\n\n -> if you only want to push layout call 'push.sh layout'\n\n"

#push layout
echo "push /SparkViewer/assets/layouts/main.spark"
adb push ./SparkViewer/assets/layouts/main.spark /sdcard/

if [ $# -eq 0 ]; then
    #push all files from dataToPush folder
    PUSHPATH="./dataToPush/*"
    for f in $PUSHPATH
    do
        echo "push $f"
        adb push $f /sdcard/
    done
fi

