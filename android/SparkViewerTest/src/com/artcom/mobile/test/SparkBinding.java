package com.artcom.mobile.test;

public class SparkBinding {
    static {
        System.loadLibrary("masl");
        System.loadLibrary("animation");
        System.loadLibrary("mar");
        System.loadLibrary("android");
        System.loadLibrary("spark");
        System.loadLibrary("testSpark");
    }
    public static native String test();
}

