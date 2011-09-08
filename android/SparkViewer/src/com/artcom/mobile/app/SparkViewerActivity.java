package com.artcom.mobile.app;


import com.artcom.mobile.Base.*;

import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;

public class SparkViewerActivity extends Activity {

    private static String LOG_TAG = "SparkViewerActivity";
    private static boolean _mySparkWorldIsLoaded = false; 
    ASLOpenGLView mView;
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        CameraTexture.init(this);
        AC_Log.setTopLevelTag(LOG_TAG);
        AC_Log.print("SparkViewer created, ready to call native [cpp logger].");
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
        NativeBinding.onPause();
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
        AC_Log.print("------------------------SparkViewer resumed");        
    }    
}
