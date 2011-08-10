package com.artcom.mobile;

//Wrapper for native library

public class BaseNativeLib {

  static {
      System.loadLibrary("base");
  }

  public static native boolean loadSpark(String theFilename);
  public static native String test();
}

