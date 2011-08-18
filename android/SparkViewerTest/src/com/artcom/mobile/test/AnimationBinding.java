package com.artcom.mobile.test;

public class AnimationBinding {
    static {
        System.loadLibrary("masl");
        System.loadLibrary("animation"); 
        System.loadLibrary("testAnimation");
    }
    public static native String test();
}
