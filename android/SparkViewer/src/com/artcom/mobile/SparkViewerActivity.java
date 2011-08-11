package com.artcom.mobile;


import com.artcom.mobile.BaseNativeLib;
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