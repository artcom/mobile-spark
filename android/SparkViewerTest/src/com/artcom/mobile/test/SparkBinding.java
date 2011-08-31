package com.artcom.mobile.test;

public class SparkBinding {
    static {
        System.loadLibrary("testSpark");
    }
    public static native String test();
}

