#!/bin/bash
CURRENT_PATH=`pwd`
echo $CURRENT_PATH
if [[ "`uname -s`" == *CYGWIN* ]]; then
    cd $(cygpath "$MOBILE_SPARK/")
else
    cd $MOBILE_SPARK
fi    

source ./android/utils.sh

if [ "$#" = "0" ]; then
    echo "we need project name as first and only parameter"
    exit 0
fi

TEMPLATE_NAME="TemplateApp"
TEMPLATE_APP_NAME="TemplateAppName"
TEMPLATE_NAME_SMALL=$(echo $TEMPLATE_NAME | awk '{print tolower($1)}')

PROJECT_NAME=$1
PROJECT_NAME_SMALL=$(echo $PROJECT_NAME | awk '{print tolower($1)}')
echo "create new project $PROJECT_NAME in namespace $PROJECT_NAME_SMALL" 

rm -rf $CURRENT_PATH/$PROJECT_NAME
cp -rf projects/TemplateApp $CURRENT_PATH/$PROJECT_NAME

cd $CURRENT_PATH/$PROJECT_NAME

rm -rf _build

cd android

if [[ "`uname -s`" == *CYGWIN* ]]; then
    HELP=$(cygpath "$MOBILE_SPARK/_build/lib")
else    
    HELP=$MOBILE_SPARK/_build/lib
fi    
OSX_SED_ADDON=""
if [[ "`uname -s`" == *Darwin* ]]; then
    OSX_SED_ADDON="\"\""
fi
REL_DIR=../$(get_relative_path `pwd` $HELP)
BUILD_LIB_CONSTANT="MOBILE_SPARK_BUILD_LIB"
sed -i $OSX_SED_ADDON 's|$BUILD_LIB_CONSTANT|$REL_DIR|g' $TEMPLATE_NAME/build.properties
sed -i $OSX_SED_ADDON 's|$BUILD_LIB_CONSTANT|$REL_DIR|g' $TEMPLATE_NAME/ant.properties
sed -i $OSX_SED_ADDON 's/$TEMPLATE_NAME/{$PROJECT_NAME/g' build.sh
sed -i $OSX_SED_ADDON 's/$TEMPLATE_NAME/$PROJECT_NAME/g' c++build.sh
sed -i $OSX_SED_ADDON 's/$TEMPLATE_NAME/$PROJECT_NAME/g' push.sh
sed -i $OSX_SED_ADDON 's/$TEMPLATE_NAME/$PROJECT_NAME/g' start.sh
mv $TEMPLATE_NAME $PROJECT_NAME
cd $PROJECT_NAME
rm -rf bin
rm -rf libs
rm -rf gen
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g AndroidManifest.xml
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g AndroidManifest.xml

mv src/com/artcom/mobile/$TEMPLATE_NAME_SMALL src/com/artcom/mobile/$PROJECT_NAME_SMALL
cd src/com/artcom/mobile/$PROJECT_NAME_SMALL
mv $TEMPLATE_NAME.java $PROJECT_NAME.java
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME.java
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME.java
cd -
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g res/values/strings.xml
cd ../..

sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g CMakeLists.txt

cd ios
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g CMakeLists.txt
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g main.m
mv $TEMPLATE_NAME"Delegate.h" $PROJECT_NAME"Delegate.h"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"Delegate.h"
mv $TEMPLATE_NAME"Delegate.m" $PROJECT_NAME"Delegate.m"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"Delegate.m"
mv $TEMPLATE_NAME"GLView.h" $PROJECT_NAME"GLView.h"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"GLView.h"
mv $TEMPLATE_NAME"GLView.mm" $PROJECT_NAME"GLView.mm"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME"GLView.mm"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"GLView.mm"

cd ../layouts
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g main.spark

cd ../src
mv $TEMPLATE_NAME"ComponentMapInitializer.h" $PROJECT_NAME"ComponentMapInitializer.h"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME"ComponentMapInitializer.h"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"ComponentMapInitializer.h"
mv $TEMPLATE_NAME"ComponentMapInitializer.cpp" $PROJECT_NAME"ComponentMapInitializer.cpp"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME"ComponentMapInitializer.cpp"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME"ComponentMapInitializer.cpp"
mv $TEMPLATE_NAME".h" $PROJECT_NAME".h"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME".h"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME".h"
mv $TEMPLATE_NAME".cpp" $PROJECT_NAME".cpp"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME_SMALL/$PROJECT_NAME_SMALL/g $PROJECT_NAME".cpp"
sed -i $OSX_SED_ADDON s/$TEMPLATE_NAME/$PROJECT_NAME/g $PROJECT_NAME".cpp"



# xxx: dirty "sed -i" fix for mac:
export OSX_SED_ADDON=""
if [[ "`uname -s`" == *Darwin* ]]; then
    rm -rf ../*"\"\""    
    rm -rf ../android/*"\"\""
    rm -rf ../android/$PROJECT_NAME/*"\"\""
    rm -rf ../android/$PROJECT_NAME/res/values/*"\"\""
    rm -rf ../android/$PROJECT_NAME/src/com/artcom/mobile/$PROJECT_NAME_SMALL/*"\"\""
    rm -rf ../ios/*"\"\""
    rm -rf ../layouts/*"\"\""
    rm -rf ../src/*"\"\""
fi


