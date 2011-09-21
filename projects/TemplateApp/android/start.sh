#/bin/bash

PROJECT_NAME="TemplateApp"
PROJECT_NAME_SMALL=$(echo $PROJECT_NAME | awk '{print tolower($1)}')

cd ..
SEVERITY=""
if [ -n "$AC_LOG_VERBOSITY" ]; then 
    SEVERITY=$AC_LOG_VERBOSITY
else
    echo "<<<<< use AC_LOG_VERBOSITY=<SEVERITY> to set a logging level"
fi

MODULE_SEVERITY=""
if [ -n "$AC_LOG_MODULE_VERBOSITY" ]; then 
    MODULE_SEVERITY=$AC_LOG_MODULE_VERBOSITY
fi

adb shell am start -a android.intent.action.MAIN -n com.artcom.mobile.$PROJECT_NAME_SMALL/com.artcom.mobile.$PROJECT_NAME_SMALL.$PROJECT_NAME --es env_0 AC_LOG_VERBOSITY="$SEVERITY" --es env_1 AC_LOG_MODULE_VERBOSITY="$MODULE_SEVERITY"

cd -

