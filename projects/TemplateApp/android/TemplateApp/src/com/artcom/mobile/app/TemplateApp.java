package com.artcom.mobile.app;

import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;

public class TemplateApp extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("TemplateApp"); 
    }
    protected static String LOG_TAG = "TemplateApp";

}
