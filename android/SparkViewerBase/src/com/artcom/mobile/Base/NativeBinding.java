// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.Base;

//Wrapper for native library
import java.io.File;
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.AssetFileDescriptor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.opengl.GLES20;
import android.opengl.GLES10;
import android.opengl.GLUtils;
import android.os.Vibrator;
import android.util.Log;

import com.artcom.mobile.Base.AC_Log;
import com.artcom.mobile.Base.Severity;
import com.artcom.mobile.Base.NativeException;
public class NativeBinding {
    public static Activity ourActivity;
    public static float ourBatteryLevel = 0.f;
    public static boolean ourAllowOrientationChange = true;

    private static final String TAG = "NativeBinding";
    public static void loadLibraries() {
        // with android ndk 7 and up, we get shared stl libs, we need to load them by hand
        try{            
            System.loadLibrary("gnustl_shared");
        }catch (UnsatisfiedLinkError t) {
        }
        System.loadLibrary("masl");
        System.loadLibrary("animation");
        System.loadLibrary("mar");
        //System.loadLibrary("android");
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

  public static int getApiVersion() {
    int apiLevel = android.os.Build.VERSION.SDK_INT;
    return apiLevel;
  }

    // http://stackoverflow.com/questions/1322510/given-an-integer-how-do-i-find-the-next-largest-power-of-two-using-bit-twiddlin
    public static double log2(double num) {
        return (Math.log(num)/Math.log(2));
    }     
    public static int next_2_power (int n)
    {
        n--;        
      return 1<<((int)log2((double)n)+1);
    }
    public static byte[] getByteArray(Bitmap bitmap) {
	    ByteArrayOutputStream bos = new ByteArrayOutputStream();
	    bitmap.compress(Bitmap.CompressFormat.PNG, 100, bos);
	    return bos.toByteArray();
	}    
    public static List<Integer> loadTextureFromFile(String theFilename, boolean theSDCardFlag, boolean theMipMapFlag) {
    List<Integer> myResult = new ArrayList<Integer>();	
    try{    
        int[] maxTextureSize = new int[1];
        GLES20.glGetIntegerv(GLES20.GL_MAX_TEXTURE_SIZE, maxTextureSize, 0);
        AC_Log.info(String.format("Max texture size = %d", maxTextureSize[0]));
        int myImageRealWidth;
        int myImageRealHeight;
        Bitmap myBitmap;
        //AC_Log.print(String.format("File is on sdcard: %s", (theSDCardFlag ? "true":"false")));
        
        BitmapFactory.Options o = new BitmapFactory.Options();
        o.inJustDecodeBounds = true;
        o.inScaled = false;
        
        InputStream is = null;
        if (theSDCardFlag) {
            is = new BufferedInputStream(new FileInputStream(theFilename));            
        } else {
            is = ourActivity.getClassLoader().getResourceAsStream(theFilename);            
        }
        BitmapFactory.decodeStream(is,null,o);
            
            //Find the correct scale value. It should be the power of 2.
            int width_tmp=o.outWidth, height_tmp=o.outHeight;
            myImageRealWidth = o.outWidth;
            myImageRealHeight = o.outHeight;
            int scale=1;
            while(true){
                if(width_tmp<=maxTextureSize[0] && height_tmp<=maxTextureSize[0]) {
                    break;
                }
                width_tmp/=2;
                height_tmp/=2;
                scale*=2;
            }
            BitmapFactory.Options o2 = new BitmapFactory.Options();
            o2.inSampleSize=scale;
            o2.inScaled = false;
            InputStream is2 = null;
            if (theSDCardFlag) {
                is2 = new BufferedInputStream(new FileInputStream(theFilename));            
            } else {
                is2 = ourActivity.getClassLoader().getResourceAsStream(theFilename);                        
            }
            myBitmap = BitmapFactory.decodeStream(is2, null, o2);
            //myBitmap = BitmapFactory.decodeResource(ourActivity.getResources(), theFilename, o2);             

        //}
        int[] textures = new int[1];
        GLES20.glGenTextures(1, textures,0);
        
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);
        //Create Nearest Filtered Texture
        if (theMipMapFlag) {
            GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR_MIPMAP_NEAREST);//GLES20.GL_LINEAR);
        } else {
              GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        }
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

        //Different possible texture parameters, e.g. GL10.GL_CLAMP_TO_EDGE
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);

        try {
            if (theMipMapFlag) {
                int myTextureWidth = next_2_power(myBitmap.getWidth());
                int myTextureHeight = next_2_power(myBitmap.getHeight());
                boolean myNPOTImage = myTextureWidth != myBitmap.getWidth() || myTextureHeight != myBitmap.getHeight();
                if (myNPOTImage) {
                    // Generate, and load up all of the mipmaps:
                    for(int level=0, height = myBitmap.getHeight(), width = myBitmap.getWidth(); true; level++) {
                        // Push the bitmap onto the GPU:
                        GLUtils.texImage2D(GL10.GL_TEXTURE_2D, level, myBitmap, 0);
                        
                        // We need to stop when the texture is 1x1:
                        if(height==1 && width==1) break;
                        
                        // Resize, and let's go again:
                        width >>= 1; height >>= 1;
                        if(width<1)  width = 1;
                        if(height<1) height = 1;
                        
                        Bitmap bmp2 = Bitmap.createScaledBitmap(myBitmap, width, height, true);
                        myBitmap.recycle();
                        myBitmap = bmp2;
                    }
                } else {                        
                    GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, myBitmap, 0);                                            
                    GLES20.glGenerateMipmap(GLES20.GL_TEXTURE_2D);
                }
            } else {
                  GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, myBitmap, 0);                                            
            }
        } catch (Exception theEx) {
          AC_Log.print(String.format("exception %s", theEx.getMessage()));
        }
                  
        //Clean up
        myBitmap.recycle();        
        myResult.add(textures[0]);
        myResult.add(myBitmap.getWidth());
        myResult.add(myBitmap.getHeight());
        myResult.add(myImageRealWidth);
        myResult.add(myImageRealHeight);
        myResult.add(myBitmap.hasAlpha() ? 1:0);
        AC_Log.info(String.format("loaded bitmap from %s realsize: %dx%d glid:%d size: %dx%d", theFilename, myImageRealWidth, myImageRealHeight, textures[0], myBitmap.getWidth(), myBitmap.getHeight()));
    }catch (Exception e) {
        myResult.add(-1);
        AC_Log.info(String.format("Image: %s on not found", theFilename));
    }
        
    return myResult;
  }
  
  private static ByteBuffer extract(Bitmap bmp, boolean theYFlipFlag) {
    AC_Log.print("extract start");
    	ByteBuffer bb = ByteBuffer.allocateDirect(bmp.getHeight() * bmp.getWidth() * 3);
    	bb.order(ByteOrder.BIG_ENDIAN);
    	IntBuffer ib = bb.asIntBuffer();
    	// Convert ARGB -> RGBA
    	int myYStart = 0;
    	if (theYFlipFlag) {
    	    myYStart = bmp.getHeight()-1;
    	}
    	for (int y = myYStart;; )
    	{
        	for (int x = 0; x < bmp.getWidth(); x++)
        	{
            	int pix = bmp.getPixel(x, bmp.getHeight() - y - 1);
            	int red = ((pix >> 16) & 0xFF);
            	int green = ((pix >> 8) & 0xFF);
            	int blue = ((pix) & 0xFF);
            	//int red = ((pix >> 24) & 0xFF);
            	//int green = ((pix >> 16) & 0xFF);
            	//int blue = ((pix >> 8) & 0xFF);
            	//int alpha = ((pix) & 0xFF);
            	// Make up alpha for interesting effect
            	//ib.put(red << 24 | green << 16 | blue << 8 | ((red + blue + green) / 3));
            	//ib.put(red << 24 | green << 16 | blue << 8 | alpha);
            	ib.put(red << 16 | green << 8 | blue);
        	}
        	if (!theYFlipFlag) {
            	y++;
            	if (y >= bmp.getHeight()) {
                	break;
            	}
        	} else {
            	y--;
            	if ( y <= 0) {
                	break;
            	}
        	}
    	}
    	bb.position(0);
    AC_Log.print("extract end");
    	return bb;
	}   
  public static List<Integer> renderText(String theMessage, int theTextureId, int theFontSize, int[] theColor, 
		                                 int maxWidth, int maxHeight, String theAlign, String theFontpath, int theLineHeight, int theStartIndex) {
    List<Integer> myResult = new ArrayList<Integer>();
    String myMessage = theMessage.substring(theStartIndex, theMessage.length());
    TextLayouter myLayouter = new TextLayouter(myMessage, maxWidth, maxHeight);
    // Draw the text
    Paint textPaint = new Paint();
    textPaint.setTextSize(theFontSize);
    textPaint.setARGB(theColor[3], theColor[0], theColor[1], theColor[2]);
    //textPaint.setShadowLayer(3, 0, 0, Color.WHITE);
    textPaint.setSubpixelText(true);
    textPaint.setAntiAlias(true);
    textPaint.setTextAlign(Paint.Align.LEFT);
    Typeface myTypeFace;
    if (theFontpath .compareTo("") != 0) {
        try{
    		myTypeFace = Typeface.createFromFile(theFontpath);
        }catch (Exception e) {
            AC_Log.info(String.format("Font: %s on sdcard not found", theFontpath));
	        try{
				myTypeFace = Typeface.createFromAsset (ourActivity.getAssets(), "fonts/" + theFontpath);
	        }catch (Exception e2) {			
		        myTypeFace = Typeface.defaultFromStyle(Typeface.NORMAL);
	            AC_Log.info(String.format("Font: %s not found in apk", theFontpath));
	        }
        }	        
    } else {
        myTypeFace = Typeface.defaultFromStyle(Typeface.NORMAL);
    }
    textPaint.setTypeface(myTypeFace);

    List<TextLine> myLines = myLayouter.createLines(textPaint, theLineHeight);
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
    }
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
          GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, myBitmap, 0);                                        
      } catch (Exception theEx) {
          AC_Log.print(String.format("exception %s", theEx.getMessage()));
      }
      
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
      if (!ourAllowOrientationChange) {
          return;
      }
      if (theOrientation.compareTo("portrait") == 0) {
          ourActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT );
      } else if (theOrientation.compareTo("landscape") == 0) {
          ourActivity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE );
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

    public static float getBatteryLevel() {
        return ourBatteryLevel;
    }

  public static void exit() {
      ourActivity.finish();    
  }
}

