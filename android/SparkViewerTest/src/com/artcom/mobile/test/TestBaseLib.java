package com.artcom.mobile.test;

import android.util.Log;

public class TestBaseLib {

    static {
        System.loadLibrary("masl");	
        System.loadLibrary("testMasl");
    }

    public static native String test();
}

