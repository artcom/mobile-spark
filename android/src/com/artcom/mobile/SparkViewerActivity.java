package com.artcom.mobile;

import com.artcom.mobile.BaseNativeLib;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class SparkViewerActivity extends Activity {
	
    private static String LOG_TAG = "SparkViewer";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(LOG_TAG, "SparkViewer created, ready to call native");     
        BaseNativeLib.loadSpark("/sdcard/test.spark");        
        Log.i(LOG_TAG, "SparkViewer native called");     
    }
    
    @Override protected void onPause() {
        super.onPause();
        Log.i(LOG_TAG, "SparkViewer paused");        
    }

    @Override protected void onResume() {
        super.onResume();
        Log.i(LOG_TAG, "SparkViewer resumed");        
    }    
}