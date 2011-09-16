package com.artcom.mobile.templateapp;

import android.os.Bundle;
import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;


public class TemplateApp extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("TemplateApp"); 
    }
    protected static String LOG_TAG = "TemplateApp";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        _myPackageExtension = ".templateapp";
        super.onCreate(savedInstanceState);
    }

}
