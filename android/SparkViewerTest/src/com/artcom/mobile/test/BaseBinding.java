package com.artcom.mobile.test;

import android.util.Log;

public class BaseBinding {

    static {
        System.loadLibrary("masl");	
        System.loadLibrary("testMasl");
    }

    public static native String test();
}

