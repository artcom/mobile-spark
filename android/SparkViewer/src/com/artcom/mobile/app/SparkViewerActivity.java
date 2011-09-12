package com.artcom.mobile.app;


import com.artcom.mobile.Base.*;

import java.lang.System;
import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;

public class SparkViewerActivity extends Activity {

    private Bundle extras_;
    private EnvMap envMap_;
    private static String GLOBAL_VERBOSITY_ENV = "AC_LOG_VERBOSITY";
    private static String LOG_TAG = "SparkViewerActivity";
    private static boolean _mySparkWorldIsLoaded = false; 
    ASLOpenGLView mView;
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        extras_ = this.getIntent().getExtras();
        envMap_ = new EnvMap();
        envMap_.init(extras_);

        CameraTexture.init(this);
        AC_Log.setTopLevelTag(LOG_TAG);
        Severity mySeverity = envMap_.hasEnv(GLOBAL_VERBOSITY_ENV) ? Severity.fromString(envMap_.getEnv(GLOBAL_VERBOSITY_ENV)) : Severity.SEV_WARNING;
        AC_Log.setSeverity(mySeverity);
        AC_Log.print("severity: " + mySeverity);
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
        Severity mySeverity = envMap_.hasEnv(GLOBAL_VERBOSITY_ENV) ? Severity.fromString(envMap_.getEnv(GLOBAL_VERBOSITY_ENV)) : Severity.SEV_WARNING;
        AC_Log.setSeverity(mySeverity);
        AC_Log.print("severity: " + mySeverity);
        AC_Log.print("------------------------SparkViewer resumed");        
    }    
    
}
