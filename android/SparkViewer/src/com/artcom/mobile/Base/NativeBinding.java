package com.artcom.mobile.Base;

//Wrapper for native library
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.List;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.opengl.GLES20;
import android.opengl.GLES10;
import android.opengl.GLUtils;

import com.artcom.mobile.Base.AC_Log;
import com.artcom.mobile.Base.Severity;
import com.artcom.mobile.Base.NativeException;
public class NativeBinding {
  
  static {    
      System.loadLibrary("masl");
      System.loadLibrary("animation");
      System.loadLibrary("mar");
      System.loadLibrary("android");
      System.loadLibrary("spark");
  }

  
  public static native void setup(long currentMillis, String apkFilePath, String layoutFile);
  public static native void onSizeChanged(int theWidth, int theHeight);
  public static native void onResume();
  public static native void initBinding();
  public static native void onEvent(String theEvent);
  
  public static native void log(Severity theSeverity, String theFilename, int theLineNumber, String theMessage);
  public static native void setLoggerTopLevelTag(String theTagString);
  public static native void setSeverity(Severity theSeverity);
  public static native boolean loadSpark(String theFilename);
  
    
  public static int renderText(String theMessage, int theTextureId, int theFontSize, int[] theColor) {	
	Bitmap myBitmap = Bitmap.createBitmap(256, 256, Bitmap.Config.ARGB_8888);
	Canvas myCanvas = new Canvas(myBitmap);
	
	int[] textures = new int[1];
	myBitmap.eraseColor(Color.TRANSPARENT);
	// Draw the text
	Paint textPaint = new Paint();
	textPaint.setTextSize(theFontSize);
	textPaint.setAntiAlias(true);
	//textPaint.setARGB(0xff, 0xff, 0x00, 0x00);
	textPaint.setARGB(theColor[3], theColor[0], theColor[1], theColor[2]);
	// draw the text centered
	myCanvas.drawText(theMessage, 16,112, textPaint);
	
	if (theTextureId == 0) {
		GLES20.glGenTextures(1, textures,0);
	} else {
	  	textures[0] = theTextureId;    		
	}
	GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);
	
	//Create Nearest Filtered Texture
	GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
	GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
	
	//Different possible texture parameters, e.g. GL10.GL_CLAMP_TO_EDGE
	GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT);
	GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_REPEAT);
	
	  try {
		  ByteBuffer bb = extract(myBitmap, false); 
		  GLES20.glTexImage2D ( GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, myBitmap.getWidth(), myBitmap.getHeight(), 0,
                                GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, bb );		  
	  } catch (Exception theEx) {
		  AC_Log.print(String.format("exception %s", theEx.getMessage()));		  
	  }	
	//Clean up
	myBitmap.recycle();
  	return textures[0];
  }	
  private static ByteBuffer extract(Bitmap bmp, boolean theYFlipFlag)
  {
	  ByteBuffer bb = ByteBuffer.allocateDirect(bmp.getHeight() * bmp.getWidth() * 4);
	  bb.order(ByteOrder.BIG_ENDIAN);
	  IntBuffer ib = bb.asIntBuffer();
	  // Convert ARGB -> RGBA
	  int myYStart = 0;
	  if (theYFlipFlag) {
		  myYStart =  bmp.getHeight()-1;		  
	  }
	  for (int y = myYStart;; )
	  {	
		  for (int x = 0; x < bmp.getWidth(); x++)
		  {
			  int pix = bmp.getPixel(x, bmp.getHeight() - y - 1);
			  int alpha = ((pix >> 24) & 0xFF);
			  int red = ((pix >> 16) & 0xFF);
			  int green = ((pix >> 8) & 0xFF);
			  int blue = ((pix) & 0xFF);			
			  // Make up alpha for interesting effect			
			  //ib.put(red << 24 | green << 16 | blue << 8 | ((red + blue + green) / 3));
			  ib.put(red << 24 | green << 16 | blue << 8 | alpha);
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
	  return bb;
  }   
  
  public static void updateCameraTexture() {
	  CameraTexture.bindTexture();
  }
  
  public static List<Integer> getCameraParams() {
	  return CameraTexture.getTextureParams();
  }
  
  public static void startCamera() {
	  CameraTexture.startCamera();
  }
  
  public static void stopCamera() {
	  CameraTexture.closeCamera();
  }
  public static boolean isCameraCapturing() {
	  return CameraTexture.isCapturing();
  }
  
}

