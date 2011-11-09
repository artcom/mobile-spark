// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.Base;


import com.artcom.mobile.Base.*;

import android.content.pm.ActivityInfo;
import android.app.Activity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.MotionEvent;
import com.artcom.mobile.Base.Sound;
import com.artcom.mobile.Base.Music;

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
    private static Music backgroundMusicPlayer;


    @Override
    public void onCreate(Bundle savedInstanceState) {
    	NativeBinding.ourActivity = this;
        AC_Log.setTopLevelTag(LOG_TAG);
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
        Severity mySeverity = envMap_.hasEnv(GLOBAL_VERBOSITY_ENV) ? Severity.fromString(envMap_.getEnv(GLOBAL_VERBOSITY_ENV)) : Severity.SEV_WARNING;
        AC_Log.setSeverity(mySeverity);
        AC_Log.print("severity: " + mySeverity);

        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        int myScreenWidth = dm.widthPixels;
        int myScreenHeight = dm.heightPixels;

        mView = new ASLOpenGLView(getApplication(), _myPackageExtension, myScreenWidth, myScreenHeight);
        setContentView(mView);
        
        //init audio
        soundPlayer = new Sound(this);
        backgroundMusicPlayer = new Music(this);
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
    
    @Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		
    	if(event.getKeyCode() == KeyEvent.KEYCODE_BACK){
    		AC_Log.print("------------------------Backbutton pressed: "+keyCode);

    		eventManager.dumbBackButtonEvent();
    		
    		return true;
    	}
    	
   		return super.onKeyDown(keyCode, event);
	}

	///////////////////////////////static audio functions called from c++  
    public static void playBackgroundMusic(String path, boolean isLoop){
        backgroundMusicPlayer.playBackgroundMusic(path, isLoop);
    }
    public static void stopBackgroundMusic(){
        backgroundMusicPlayer.stopBackgroundMusic();
    }
    public static void pauseBackgroundMusic(){
        backgroundMusicPlayer.pauseBackgroundMusic();
    }
    public static void resumeBackgroundMusic(){
        backgroundMusicPlayer.resumeBackgroundMusic();
    }
    public static void rewindBackgroundMusic(){
        backgroundMusicPlayer.rewindBackgroundMusic();
    }
    public static boolean isBackgroundMusicPlaying(){
        return backgroundMusicPlayer.isBackgroundMusicPlaying();
    }
    public static float getBackgroundMusicVolume(){
        return backgroundMusicPlayer.getBackgroundVolume();
    }
    public static void setBackgroundMusicVolume(float volume){
        backgroundMusicPlayer.setBackgroundVolume(volume);
    }
    public static int playEffect(String path){
        return soundPlayer.playEffect(path);
    }
    public static void stopEffect(int soundId){
        soundPlayer.stopEffect(soundId);
    }
    public static float getEffectsVolume(){
        return soundPlayer.getEffectsVolume();
    }
    public static void setEffectsVolume(float volume){
        soundPlayer.setEffectsVolume(volume);
    }
    public static void preloadEffect(String path){
        soundPlayer.preloadEffect(path);
    }
    public static void unloadEffect(String path){
        soundPlayer.unloadEffect(path);
    }
    public static void end(){
        backgroundMusicPlayer.end();
        soundPlayer.end();
    }
}
