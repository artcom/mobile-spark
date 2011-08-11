package com.artcom.mobile;

//Wrapper for native library

public class BaseNativeLib {

  static {
      System.loadLibrary("base");
  }

  //testing
  public static native boolean loadSpark(String theFilename);
  //public static native String test();
  
  public static native void setup(String apkFilePath, String layoutFile);
  public static native void onFrame();
  public static native void onTouch();
}

