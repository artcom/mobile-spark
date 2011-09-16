package com.artcom.mobile.app;

import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;

public class DemoApp extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("DemoApp"); 
    }
    protected static String LOG_TAG = "DemoApp";

}
