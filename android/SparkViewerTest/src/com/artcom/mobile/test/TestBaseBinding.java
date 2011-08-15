package com.artcom.mobile.test;

import android.util.Log;

public class TestBaseBinding {

    static {
        System.loadLibrary("masl");	
        System.loadLibrary("testMasl");
    }

    public static native String test();
}

