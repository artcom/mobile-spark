package com.artcom.mobile.test;

import android.util.Log;

public class TestAndroidLib {

    static {
        System.loadLibrary("base");	
        System.loadLibrary("testBase");

        System.loadLibrary("zip");
        System.loadLibrary("android");
        System.loadLibrary("testAndroid");
    }

    public static native String test();
}
