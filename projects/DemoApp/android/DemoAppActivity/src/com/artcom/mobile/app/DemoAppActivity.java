package com.artcom.mobile.app;

import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;

public class DemoAppActivity extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("demoapp"); 
    }
    protected static String LOG_TAG = "DemoAppActivity";

}
