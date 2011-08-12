package com.artcom.mobile;


import com.artcom.mobile.BaseBinding;
import com.artcom.mobile.Base.AC_Logger;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class SparkViewerActivity extends Activity {
	
    private static String LOG_TAG = "SparkViewerActivity";
    ASLOpenGLView mView;
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mView = new ASLOpenGLView(getApplication());
        setContentView(mView);
        
        //testing
        Log.i(LOG_TAG, "SparkViewer created, ready to call native.");  
        AC_Logger.print("SparkViewer created, ready to call native [cpp logger].");
        BaseBinding.loadSpark("/sdcard/test.spark");        
        Log.i(LOG_TAG, "SparkViewer native called");     
    }
    
    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
        Log.i(LOG_TAG, "SparkViewer paused");        
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
        Log.i(LOG_TAG, "SparkViewer resumed");        
    }    
}