package com.artcom.mobile.test;

import android.util.Log;

public class TestAndroidLib {

    static {
        System.loadLibrary("masl");
        System.loadLibrary("android"); 
        System.loadLibrary("testAndroid");
    }

    public static native String test(String apkPath);
}
