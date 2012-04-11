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
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

public class SparkViewerActivity extends Activity {

	protected String LOG_TAG = "SparkViewerActivity";
    private SparkOpenGLViewDelegate mViewDelegate;
    protected String _myPackageName; //should be set by child classes  

    @Override
    public void onCreate(Bundle savedInstanceState) {
    	mViewDelegate = new SparkOpenGLViewDelegate(this);
    	
        AC_Log.setTopLevelTag(LOG_TAG);
        AC_Log.print("----------------------------------------------------------------------");
        AC_Log.print("-*************************SparkViewer********************************-");
        AC_Log.print("----------------------------------------------------------------------");
        super.onCreate(savedInstanceState);

        mViewDelegate.init(this, _myPackageName);
        setContentView(mViewDelegate.getView());
        setupBatteryLevelReceiver();
    }

    private void setupBatteryLevelReceiver () {
        BroadcastReceiver batteryLevelReceiver = new BroadcastReceiver() {
            public void onReceive(Context context, Intent intent) {
                int level = intent.getIntExtra("level", 0);
                int scale = intent.getIntExtra("scale", 1);
                level = (level * 100) / scale;
                NativeBinding.ourBatteryLevel = level;
            }
        };
        IntentFilter batteryLevelFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        registerReceiver(batteryLevelReceiver, batteryLevelFilter);
    }

    public boolean onTouchEvent(MotionEvent event) {
        return mViewDelegate.dumpTouchEvent(event) ;
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
        mViewDelegate.onPause();
    }

    @Override protected void onStop() {
        AC_Log.print("--------------------- on Stop");
        super.onStop();
    }

    @Override protected void onResume() {
        AC_Log.print("------------------------SparkViewer resumed");
        super.onResume();
        mViewDelegate.onResume();
    }
    
    @Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		
    	if(event.getKeyCode() == KeyEvent.KEYCODE_BACK){
    		AC_Log.print("------------------------Backbutton pressed: "+keyCode);

    		mViewDelegate.dumbBackButtonEvent();
    		
    		return true;
    	}
    	
   		return super.onKeyDown(keyCode, event);
	}

	
}
