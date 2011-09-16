package com.artcom.mobile.acprojectview;

import android.os.Bundle;
import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;


public class ACProjectView extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("ACProjectView"); 
    }
    protected static String LOG_TAG = "ACProjectView";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        _myPackageExtension = ".acprojectview";
        super.onCreate(savedInstanceState);
    }

}
