package com.artcom.mobile.test;


public class BaseBinding {

    static {
        System.loadLibrary("masl");	
        System.loadLibrary("testMasl");
    }

    public static native String test();
}

