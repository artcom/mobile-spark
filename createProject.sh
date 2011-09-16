#!/bin/bash

if [ "$#" = "0" ]; then
    echo "we need project name as first and only parameter"
    exit 0
fi

TEMPLATE_NAME="TemplateApp"
TEMPLATE_NAME_SMALL=$(echo $TEMPLATE_NAME | awk '{print tolower($1)}')

PROJECT_NAME=$1
PROJECT_NAME_SMALL=$(echo $PROJECT_NAME | awk '{print tolower($1)}')
echo "create new project $PROJECT_NAME in namespace $PROJECT_NAME_SMALL" 

rm -rf projects/$PROJECT_NAME
cp -rf projects/TemplateApp projects/$PROJECT_NAME

cd projects/$PROJECT_NAME

rm -rf _build

cd android
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g build.sh
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g push.sh
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g start.sh
mv $TEMPLATE_NAME $PROJECT_NAME
cd $PROJECT_NAME
rm -rf bin
rm -rf libs
rm -rf gen
sed -i s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g AndroidManifest.xml
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g AndroidManifest.xml

mv src/com/artcom/mobile/$TEMPLATE_NAME_SMALL src/com/artcom/mobile/$PROJECT_NAME_SMALL
cd src/com/artcom/mobile/$PROJECT_NAME_SMALL
mv $TEMPLATE_NAME.java $PROJECT_NAME.java
sed -i s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME.java
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME.java
cd -
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g res/values/strings.xml
cd ../..

sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g CMakeLists.txt

cd ios
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g CMakeLists.txt
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g main.m
mv $TEMPLATE_NAME"Delegate.h" $PROJECT_NAME"Delegate.h"
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"Delegate.h"
mv $TEMPLATE_NAME"Delegate.m" $PROJECT_NAME"Delegate.m"
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"Delegate.m"
mv $TEMPLATE_NAME"GLView.h" $PROJECT_NAME"GLView.h"
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"GLView.h"
mv $TEMPLATE_NAME"GLView.mm" $PROJECT_NAME"GLView.mm"
sed -i s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME"GLView.mm"
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"GLView.mm"

cd ../layouts
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g main.spark

cd ../src
mv $TEMPLATE_NAME"ComponentMapInitializer.h" $PROJECT_NAME"ComponentMapInitializer.h"
sed -i s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME"ComponentMapInitializer.h"
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"ComponentMapInitializer.h"
mv $TEMPLATE_NAME"ComponentMapInitializer.cpp" $PROJECT_NAME"ComponentMapInitializer.cpp"
sed -i s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME"ComponentMapInitializer.cpp"
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"ComponentMapInitializer.cpp"
mv $TEMPLATE_NAME".h" $PROJECT_NAME".h"
sed -i s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME".h"
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME".h"
mv $TEMPLATE_NAME".cpp" $PROJECT_NAME".cpp"
sed -i s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME".cpp"
sed -i s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME".cpp"

