#/bin/bash

SEVERITY=""
if [ -n "$AC_LOG_VERBOSITY" ]; then 
    SEVERITY=$AC_LOG_VERBOSITY
else
    echo "<<<<< use AC_LOG_VERBOSITY=<SEVERITY> to set a logging level"
fi

adb shell am start -a android.intent.action.MAIN -n com.artcom.mobile/com.artcom.mobile.app.TemplateActivity --es env_0 AC_LOG_VERBOSITY="$SEVERITY"
