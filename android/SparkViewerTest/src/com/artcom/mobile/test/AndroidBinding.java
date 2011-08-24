package com.artcom.mobile.test;

import android.util.Log;

public class AndroidBinding {

    static {
        //System.loadLibrary("masl");
        //System.loadLibrary("mar");
        //System.loadLibrary("android"); 
        System.loadLibrary("testAndroid");
    }

    public static native String test(String apkPath);
}
