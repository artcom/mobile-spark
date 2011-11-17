package com.artcom.mobile.Base;

import android.app.Activity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.MotionEvent;


public class SparkOpenGLViewDelegate {
	
    private SparkOpenGLView mView;

	public boolean sparkWorldIsLoaded = false;
    private EventManager eventManager;
    private Sensors sensors;
	private Bundle extras_;
    private EnvMap envMap_;
   
    private static String GLOBAL_VERBOSITY_ENV = "AC_LOG_VERBOSITY";

    
    public SparkOpenGLViewDelegate(Activity theActivity) {
    	NativeBinding.ourActivity = theActivity;
        eventManager = new EventManager();
        sensors = new Sensors(theActivity);
        sensors.enable(Sensors.ORIENTATION);
        sensors.enable(Sensors.LIGHT);
        sensors.enable(Sensors.GYROSCOPE);
	}

    public void init(Activity theActivity, String thePackageName) {
        extras_ = theActivity.getIntent().getExtras();
    	envMap_ = new EnvMap();
        envMap_.init(extras_);
        Severity mySeverity = envMap_.hasEnv(GLOBAL_VERBOSITY_ENV) ? Severity.fromString(envMap_.getEnv(GLOBAL_VERBOSITY_ENV)) : Severity.SEV_WARNING;
        AC_Log.setSeverity(mySeverity);
        AC_Log.print("severity: " + mySeverity);
       
        CameraTexture.init(theActivity);
        AudioDelegate audioDelegate = new AudioDelegate(theActivity);

        DisplayMetrics dm = new DisplayMetrics();
        theActivity.getWindowManager().getDefaultDisplay().getMetrics(dm);
        int myScreenWidth = dm.widthPixels;
        int myScreenHeight = dm.heightPixels;
        
        mView = new SparkOpenGLView(theActivity.getApplication(), this, thePackageName, myScreenWidth, myScreenHeight);
    }
    
    public void onPause() {
        mView.onPause();
        NativeBinding.onPause();
        CameraTexture.closeCamera();
        Sensors.disableAllSensors();
    }

    public void onResume() {
        mView.onResume();
    }

    public boolean dumpTouchEvent(MotionEvent event) {
        return eventManager.dumpTouchEvent(event) ;
    }
    
    public void dumbBackButtonEvent() {
        eventManager.dumbBackButtonEvent();
    }

    public SparkOpenGLView getView() {
        return mView;
    }
}
