package com.artcom.mobile.Base;


import com.artcom.mobile.Base.*;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;

public class SparkViewerActivity extends Activity {

    private Bundle extras_;
    private EnvMap envMap_;
    private static String GLOBAL_VERBOSITY_ENV = "AC_LOG_VERBOSITY";
    protected static String LOG_TAG = "SparkViewerActivity";
    private static boolean _mySparkWorldIsLoaded = false; 
    ASLOpenGLView mView;
    private EventManager eventManager;
    private Sensors sensors;
    
    
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        extras_ = this.getIntent().getExtras();
        envMap_ = new EnvMap();
        envMap_.init(extras_);
     
        CameraTexture.init(this);
        eventManager = new EventManager();
        sensors = new Sensors(this);
        sensors.enable(Sensors.GYROSCOPE);
        //sensors.enable(Sensors.MAGNETIC_FIELD);
        AC_Log.setTopLevelTag(LOG_TAG);
        Severity mySeverity = envMap_.hasEnv(GLOBAL_VERBOSITY_ENV) ? Severity.fromString(envMap_.getEnv(GLOBAL_VERBOSITY_ENV)) : Severity.SEV_WARNING;
        AC_Log.setSeverity(mySeverity);
        AC_Log.print("severity: " + mySeverity);
        AC_Log.print("SparkViewer created, ready to call native [cpp logger]. ");
        mView = new ASLOpenGLView(getApplication(), !_mySparkWorldIsLoaded);        
        setContentView(mView);
    }
    
    public boolean onTouchEvent(MotionEvent event) {
    	return eventManager.dumpTouchEvent(event) ;
    }
    
    @Override protected void onStart() {
        AC_Log.print("----------------------SparkViewer started");     
        super.onStart();
        _mySparkWorldIsLoaded = true;
    }
    
    @Override protected void onPause() {
        AC_Log.print("----------------------SparkViewer paused");
        super.onPause();
        mView.onPause();
        String myEvent = "<StageEvent type='pause'/>";
        NativeBinding.onEvent(myEvent);
        CameraTexture.closeCamera();
        Sensors.disableAllSensors();
        AC_Log.print("----------------------SparkViewer paused");
    }
    @Override protected void onStop() {
        AC_Log.print("--------------------- on Stop");
        super.onStop();
    }

    @Override protected void onResume() {
        AC_Log.print("------------------------SparkViewer resumed");        
        super.onResume();
        mView.onResume();
        Severity mySeverity = envMap_.hasEnv(GLOBAL_VERBOSITY_ENV) ? Severity.fromString(envMap_.getEnv(GLOBAL_VERBOSITY_ENV)) : Severity.SEV_WARNING;
        AC_Log.setSeverity(mySeverity);
        AC_Log.print("severity: " + mySeverity);
    }    
    
}
