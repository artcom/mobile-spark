package com.artcom.mobile.demoapp;

import android.os.Bundle;
import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;

public class DemoApp extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("DemoApp"); 
        NativeBinding.loadLibrary("libDemoApp.so"); 
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        LOG_TAG = "DemoApp";
        _myPackageExtension = ".demoapp";
        super.onCreate(savedInstanceState);
    }
}
