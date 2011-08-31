package com.artcom.mobile.Base;

//Wrapper for native library
import com.artcom.mobile.Base.AC_Log;
public class NativeBinding {
  
  static {    
      System.loadLibrary("masl");
      System.loadLibrary("animation");
      System.loadLibrary("mar");
      System.loadLibrary("android");
      System.loadLibrary("spark");
  }

  
  public static native void setup(long currentMillis, String apkFilePath, String layoutFile);
  public static native void onFrame(long currentMillis);
  public static native void onSizeChanged(int theWidth, int theHeight);
  public static native void onTouch();
  public static native void onPause();
  public static native void onResume();
  public static native void onEvent(String theEvent);

  
  public static native void log(int theSeverity, String theFilename, int theLineNumber, String theMessage);
  public static native void setLoggerTopLevelTag(String theTagString);
  public static native boolean loadSpark(String theFilename);
}

