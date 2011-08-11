package com.artcom.mobile;

//Wrapper for native library

public class BaseNativeLib {

  static {
      System.loadLibrary("masl");
      System.loadLibrary("android");
      System.loadLibrary("mar");
      System.loadLibrary("spark");
  }

  
  public static native void setup(String apkFilePath, String layoutFile);
  public static native void onFrame();
  public static native void onTouch();
}

