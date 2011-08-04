package com.artcom.mobile;

//Wrapper for native library

public class BaseNativeLib {

  static {
      System.loadLibrary("CppEntry");
  }

  public static native void loadSpark(String theFilename);
}

