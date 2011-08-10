package com.artcom.mobile.test;

import android.util.Log;

public class TestBaseLib {

static {
    System.loadLibrary("base");	
    System.loadLibrary("testBase");
}

public static native String test();
}

