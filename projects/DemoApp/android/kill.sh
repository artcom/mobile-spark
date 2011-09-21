#/bin/bash

PROJECT_NAME="DemoApp"
PROJECT_NAME_SMALL=$(echo $PROJECT_NAME | awk '{print tolower($1)}')

PID=$(adb shell ps | grep $PROJECT_NAME_SMALL | awk '{ print $2 }')
python ../../../android/killProcess.py $PID

