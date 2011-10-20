package com.artcom.mobile.Base;


import com.artcom.mobile.Base.*;

import android.content.pm.ActivityInfo;
import android.app.Activity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import com.artcom.mobile.Base.Sound;

public class SparkViewerActivity extends Activity {

    private Bundle extras_;
    private EnvMap envMap_;
    private static String GLOBAL_VERBOSITY_ENV = "AC_LOG_VERBOSITY";
    protected String LOG_TAG = "SparkViewerActivity";
    protected String _myPackageExtension; //should be set by child classes
    public boolean _mySparkWorldIsLoaded = false;
    ASLOpenGLView mView;
    private EventManager eventManager;
    private Sensors sensors;
    private static Sound soundPlayer;

    public static int playEffect(String path){
        return soundPlayer.playEffect(path);
    }    
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	NativeBinding.ourActivity = this;
        AC_Log.print("----------------------------------------------------------------------");
        AC_Log.print("-*************************SparkViewer********************************-");
        AC_Log.print("----------------------------------------------------------------------");
        super.onCreate(savedInstanceState);
        extras_ = this.getIntent().getExtras();
        envMap_ = new EnvMap();
        envMap_.init(extras_);

        CameraTexture.init(this);
        eventManager = new EventManager();
        sensors = new Sensors(this);
        sensors.enable(Sensors.ORIENTATION);
        sensors.enable(Sensors.LIGHT);
        sensors.enable(Sensors.GYROSCOPE);
        AC_Log.setTopLevelTag(LOG_TAG);
        Severity mySeverity = envMap_.hasEnv(GLOBAL_VERBOSITY_ENV) ? Severity.fromString(envMap_.getEnv(GLOBAL_VERBOSITY_ENV)) : Severity.SEV_WARNING;
        AC_Log.setSeverity(mySeverity);
        AC_Log.print("severity: " + mySeverity);

        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        int myScreenWidth = dm.widthPixels;
        int myScreenHeight = dm.heightPixels;

        mView = new ASLOpenGLView(getApplication(), _myPackageExtension, myScreenWidth, myScreenHeight);
        setContentView(mView);
        soundPlayer = new Sound(this);

    }

    public boolean onTouchEvent(MotionEvent event) {
        return eventManager.dumpTouchEvent(event) ;
    }

    @Override protected void onRestart() {
        AC_Log.print("----------------------SparkViewer restarted");
        super.onRestart();
    }
    @Override protected void onStart() {
        AC_Log.print("----------------------SparkViewer started");
        super.onStart();
    }

    @Override protected void onPause() {
        AC_Log.print("----------------------SparkViewer paused");
        super.onPause();
        mView.onPause();
        NativeBinding.onPause();
        CameraTexture.closeCamera();
        Sensors.disableAllSensors();
    }
    @Override protected void onStop() {
        AC_Log.print("--------------------- on Stop");
        super.onStop();
    }

    @Override protected void onResume() {
        AC_Log.print("------------------------SparkViewer resumed");
        super.onResume();
        mView.onResume();
    }

}
