package com.artcom.mobile.test;

public class TestBaseLib {

    static {
        System.loadLibrary("masl");	
        System.loadLibrary("testMasl");
    }

    public static native String test();
}

