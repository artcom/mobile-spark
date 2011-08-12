package com.artcom.mobile.Base;

//Wrapper for native library

public class NativeBinding {

  static {
      System.loadLibrary("masl");
      System.loadLibrary("android");
      System.loadLibrary("mar");
      System.loadLibrary("spark");
  }

  
  public static native void setup(String apkFilePath, String layoutFile);
  public static native void onFrame();
  public static native void onTouch();
  public static native void log(int theSeverity, String theFilename, int theLineNumber, String theMessage);
  public static native boolean loadSpark(String theFilename);
}

