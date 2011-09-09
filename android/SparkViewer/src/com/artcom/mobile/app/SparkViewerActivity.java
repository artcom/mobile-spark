package com.artcom.mobile.app;


import com.artcom.mobile.Base.*;

import java.lang.System;
import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;

public class SparkViewerActivity extends Activity {

    private static String GLOBAL_VERBOSITY_ENV = "AC_LOG_VERBOSITY";
    private static String LOG_TAG = "SparkViewerActivity";
    private static boolean _mySparkWorldIsLoaded = false; 
    ASLOpenGLView mView;
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        CameraTexture.init(this);
        parseEnvironmentVariables();
        AC_Log.setTopLevelTag(LOG_TAG);
        AC_Log.setSeverity(Severity.SEV_WARNING);
        AC_Log.print("SparkViewer created, ready to call native [cpp logger]. ");
        mView = new ASLOpenGLView(getApplication(), !_mySparkWorldIsLoaded);        
        setContentView(mView);         
    }
    
    @Override protected void onStart() {
        super.onStart();
        _mySparkWorldIsLoaded = true;
        AC_Log.print("----------------------SparkViewer started");     
    }
    
    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
        String myEvent = "<StageEvent type='pause'/>";
        NativeBinding.onEvent(myEvent);
        CameraTexture.closeCamera();
        AC_Log.print("----------------------SparkViewer paused");
    }
    @Override protected void onStop() {
        super.onStop();
        AC_Log.print("--------------------- on Stop");
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
        String myVerbosity = System.getenv("ANDROID_LOG_TAGS");
        AC_Log.setSeverity(Severity.SEV_WARNING);
        AC_Log.print("severity: " + myVerbosity);
        AC_Log.print("------------------------SparkViewer resumed");        
    }    

    private void parseEnvironmentVariables() {
        /*Bundle extras = this.getIntent().getExtras();
        if ( extras != null ) {
            String myEnvVar = extras.getString("env_0");
            for (int i = 1; myEnvVar != null; ++i) {
                _myEnvironmentVariables
            }
            if ( extras.containsKey ( GLOBAL_VERBOSITY_ENV ) ) {
                AC_Log.print ( GLOBAL_VERBOSITY_ENV + "=" + extras.getString ( GLOBAL_VERBOSITY_ENV ) );
            }
        }*/
    }
    
}
