package com.artcom.mobile.app;


import com.artcom.mobile.Base.AC_Log;
import com.artcom.mobile.Base.*;

import android.app.Activity;
import android.os.Bundle;

public class SparkViewerActivity extends Activity {
	
    private static String LOG_TAG = "SparkViewerActivity";
    ASLOpenGLView mView;
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        AC_Log.setTopLevelTag(LOG_TAG);
        AC_Log.print("SparkViewer created, ready to call native [cpp logger].");
        mView = new ASLOpenGLView(getApplication());
        setContentView(mView);        
    }
    
    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
        AC_Log.print("----------------------SparkViewer paused");     
    }
    @Override protected void onStop() {
        super.onStop();
        AC_Log.print("--------------------- on Stop");
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
        AC_Log.print("------------------------SparkViewer resumed");        
    }    
}