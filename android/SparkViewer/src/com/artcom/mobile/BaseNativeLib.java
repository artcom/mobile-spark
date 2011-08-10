package com.artcom.mobile;

//Wrapper for native library

public class BaseNativeLib {

  static {
      System.loadLibrary("base");
      System.loadLibrary("zip");
      System.loadLibrary("android");
  }

  public static native boolean loadSpark(String theFilename);
  public static native String test();
}

