package com.artcom.mobile.Base;

//Wrapper for native library
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.opengl.GLES20;
import android.opengl.GLES10;
import android.opengl.GLUtils;
import android.os.Vibrator;

import com.artcom.mobile.Base.AC_Log;
import com.artcom.mobile.Base.Severity;
import com.artcom.mobile.Base.NativeException;
public class NativeBinding {
    public static SparkViewerActivity ourActivity;


  public static void loadLibraries() {
      System.loadLibrary("masl");
      System.loadLibrary("animation");
      System.loadLibrary("mar");
      System.loadLibrary("android");
      System.loadLibrary("spark");
  }

  public static native void setup(long currentMillis, String apkFilePath, int theScreenWidth, int theScreenHeight);
  public static native void putEnv(String theEnvVar);
  public static native void onPause();
  public static native void onResume();
  public static native void initBinding();
  public static native void onEvent(String theEvent);
  public static native void handleEvents();

  public static native void log(Severity theSeverity, String theFilename, int theLineNumber, String theMessage);
  public static native void setLoggerTopLevelTag(String theTagString);
  public static native void setSeverity(Severity theSeverity);


  public static List<Integer> renderText(String theMessage, int theTextureId, int theFontSize, int[] theColor, 
		                                 int maxWidth, int maxHeight, String theAlign, String theFontpath, int theLineHeight, int theStartIndex) {
    List<Integer> myResult = new ArrayList<Integer>();
    String myMessage = theMessage.substring(theStartIndex, theMessage.length());
    //AC_Log.print(String.format("Message length: %s", myMessage));
    TextLayouter myLayouter = new TextLayouter(myMessage, theFontSize, maxWidth, maxHeight);
    // Draw the text
    Paint textPaint = new Paint();
    textPaint.setTextSize(theFontSize);
    textPaint.setARGB(theColor[3], theColor[0], theColor[1], theColor[2]);
    //textPaint.setShadowLayer(3, 0, 0, Color.WHITE);
    textPaint.setSubpixelText(true);
    textPaint.setAntiAlias(true);
    textPaint.setTextAlign(Paint.Align.LEFT);
    Typeface myTypeFace;
    //myTypeFace = Typeface.defaultFromStyle(Typeface.NORMAL);
    if (theFontpath .compareTo("") != 0) {
        myTypeFace = Typeface.createFromFile(theFontpath);
    } else {
        myTypeFace = Typeface.defaultFromStyle(Typeface.NORMAL);
    }
    textPaint.setTypeface(myTypeFace);

    List<TextLine> myLines = myLayouter.createLines(textPaint, theLineHeight);
    //AC_Log.print(String.format("CanvasHeight: %d, %d", myLayouter.getCanvasWidth(), myLayouter.getCanvasHeight()));
    Bitmap myBitmap = Bitmap.createBitmap(Math.max(1, myLayouter.getCanvasWidth()), Math.max(1, myLayouter.getCanvasHeight()), Bitmap.Config.ARGB_8888);
    Canvas myCanvas = new Canvas(myBitmap);
    myBitmap.eraseColor(Color.TRANSPARENT);

    // draw the text
    for (int i = 0; i < myLines.size(); i++) {
        int myXOffset = 0;
        if (theAlign.compareTo("center") == 0) {
            myXOffset = (int) ((myBitmap.getWidth() - myLines.get(i)._myWidth) /2.0);
        } else if (theAlign.compareTo("right") == 0) {
            myXOffset = (int) (myBitmap.getWidth() - myLines.get(i)._myWidth);
        }
        myCanvas.drawText(myLines.get(i)._myLineOfText, myLines.get(i)._myXPos + myXOffset, myLines.get(i)._myYPos, textPaint);
        //AC_Log.print(String.format("Line %d '%s' at (%d,%d)" , i, myLines.get(i)._myLineOfText, myLines.get(i)._myXPos, myLines.get(i)._myYPos));
    }
	//long myTextureStart = System.currentTimeMillis();    	
    int[] textures = new int[1];
    if (theTextureId == 0) {
        GLES20.glGenTextures(1, textures,0);
    } else {
        textures[0] = theTextureId;
    }
    GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);

    //Create Nearest Filtered Texture
    GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);

    //Different possible texture parameters, e.g. GL10.GL_CLAMP_TO_EDGE
    GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
    GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);

      try {
          ByteBuffer bb = extract(myBitmap, false);
          GLES20.glTexImage2D ( GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, myBitmap.getWidth(), myBitmap.getHeight(), 0,
                                GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, bb );
      } catch (Exception theEx) {
          AC_Log.print(String.format("exception %s", theEx.getMessage()));
      }
      //AC_Log.print(String.format("rendertext glzeugs %d", (System.currentTimeMillis() - myTextureStart)));
      
    //Clean up
    myBitmap.recycle();

    myResult.add(textures[0]);
    myResult.add(myBitmap.getWidth());
    myResult.add(myBitmap.getHeight());
    if (myLayouter.getRenderedGlyphIndex() > 0) {
        myResult.add(theStartIndex + myLayouter.getRenderedGlyphIndex());   
    } else {
    	// we are all through the text
    	myResult.add(-1);
    }
    return myResult;
  }
  private static ByteBuffer extract(Bitmap bmp, boolean theYFlipFlag)
  {
      ByteBuffer bb = ByteBuffer.allocateDirect(bmp.getHeight() * bmp.getWidth() * 4);
      bb.order(ByteOrder.BIG_ENDIAN);
      IntBuffer ib = bb.asIntBuffer();
      // Convert ARGB -> RGBA
      int myYStart = 0;
      int myXEnd = bmp.getWidth();
      int myHeight = bmp.getHeight();
      //if (theYFlipFlag) {
      //    myYStart =  bmp.getHeight()-1;
      //}
      for (int y = myYStart; y < myHeight; )
      {
          for (int x = 0; x < myXEnd; ++x)
          {
              int pix = bmp.getPixel(x, myHeight - y - 1);
              int a = ((pix >> 24) & 0xFF);
              ib.put(a | pix << 8);
              //int alpha = ((pix >> 24) & 0xFF);
              //int red = ((pix >> 16) & 0xFF);
              //int green = ((pix >> 8) & 0xFF);
              //int blue = ((pix) & 0xFF);
              //// Make up alpha for interesting effect
              ////ib.put(red << 24 | green << 16 | blue << 8 | ((red + blue + green) / 3));
              //ib.put(red << 24 | green << 16 | blue << 8 | alpha);
          }
          ++y;
          //if (!theYFlipFlag) {
          //    y++;
          //    if (y >= bmp.getHeight()) {
          //        break;
          //    }
          //} else {
          //    y--;
          //    if ( y <= 0) {
          //        break;
          //    }
          //}
      }
      bb.position(0);
      return bb;
  }

  public static void updateCameraTexture() {
      CameraTexture.bindTexture();
  }

  public static List<Integer> getCameraParams() {
      return CameraTexture.getTextureParams();
  }

  public static void startCamera(boolean theColorConversionFlag) {
      CameraTexture.startCamera(theColorConversionFlag);
  }

  public static void stopCamera() {
      CameraTexture.closeCamera();
  }

  public static boolean isCameraCapturing() {
      return CameraTexture.isCapturing();
  }

  public static void freezeOrientation(String theOrientation) {
     if (theOrientation.compareTo("portrait") == 0) {
    	 ourActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT );
     } else if (theOrientation.compareTo("landscape") == 0) {
    	 ourActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE );
     }
  }

  public static void vibrate(int theDurationMillisec) {
      try {	  
		  Vibrator vibrator = (Vibrator)ourActivity.getSystemService(Context.VIBRATOR_SERVICE);
		  if (vibrator != null) {
			  vibrator.vibrate(theDurationMillisec);
		  }
      } catch (Exception theEx) {
          AC_Log.print(String.format("exception %s", theEx.getMessage()));
      }
	  
  }

}

