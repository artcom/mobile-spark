package com.artcom.mobile.app;

import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;

public class ProjectViewActivity extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("projectviewapp"); 
    }
    protected static String LOG_TAG = "ProjectViewActivity";

}
