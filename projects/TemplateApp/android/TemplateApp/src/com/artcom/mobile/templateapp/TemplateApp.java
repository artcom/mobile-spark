package com.artcom.mobile.templateapp;

import android.os.Bundle;
import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;


public class TemplateApp extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("TemplateApp"); 
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        LOG_TAG = "TemplateApp";
        _myPackageExtension = ".templateapp";
        super.onCreate(savedInstanceState);
    }

}
