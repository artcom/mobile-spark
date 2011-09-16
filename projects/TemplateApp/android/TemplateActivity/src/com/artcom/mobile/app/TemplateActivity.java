package com.artcom.mobile.app;

import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;

public class TemplateActivity extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("templateapp"); 
    }
    protected static String LOG_TAG = "TemplateActivity";

}
