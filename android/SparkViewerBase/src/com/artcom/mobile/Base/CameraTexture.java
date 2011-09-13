package com.artcom.mobile.Base;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import javax.microedition.khronos.opengles.GL10;
import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;

public class CameraTexture implements SurfaceHolder.Callback {
	
    private static CameraTexture INSTANCE;
	private static Activity theActivity;
	private static SurfaceView theSurfaceView;
	private static GL10 gl;
	private static boolean _myColorConversionFlag = true;
	private SurfaceHolder _mySurfaceHolder;
	private Camera _myCamera;
	private int _myCamWidth, _myCamHeight, _myTextureWidth, _myTextureHeight;
    private int _myTextureID;
	private int[] _myCameraTextures;
	private static byte _myCamData[] = new byte[0];
	private boolean _newFrameAvailable=false;
	
	//--------- STATIC --------------------------------------------------------
	public static CameraTexture init(Activity activity) {
		if(INSTANCE==null) INSTANCE = new CameraTexture(activity);
	    return INSTANCE;
	}
	public static void initWithContext(GL10 glContext) {
		gl=glContext;
	}
	//-------------------------------------------------------------------------
	public static void startCamera(boolean theColorConversionFlag) {
		_myColorConversionFlag = theColorConversionFlag;
		AC_Log.print(String.format("Start Camera with cpu yuv-rgb color conversion : %b", theColorConversionFlag));
		if (theActivity == null) {
			AC_Log.print("CameraTexure not initialized, please call init() before");
			return;
		}
		if (gl==null) {
			AC_Log.print("CameraTexure has no openGL-context, please call initWithContext() before");
			return;
		}
		init(theActivity);
		INSTANCE.start();
	}
	//-------------------------------------------------------------------------
    public static void closeCamera() {
    	AC_Log.print("--------------------- close Cam");
    	if (INSTANCE != null) INSTANCE.close();
    	INSTANCE=null;
    }
	//-------------------------------------------------------------------------
    public static void bindTexture() {
    	if (INSTANCE != null ) INSTANCE.bind();
    }
    public static boolean isCapturing() {
    	return (INSTANCE != null) && (INSTANCE._myCamera != null);
    }
	//-------------------------------------------------------------------------
    // returns [0]-textureID; [1]-preview width; [2]-preview height; [3]-texture width; [4]-texture height
    public static List<Integer> getTextureParams() {
		if (INSTANCE == null ) return null;
		List<Integer> myResult = new ArrayList<Integer>();
		myResult.add(INSTANCE._myTextureID);
		myResult.add(INSTANCE._myCamWidth);
		myResult.add(INSTANCE._myCamHeight);
		myResult.add(INSTANCE._myTextureWidth);		  
		myResult.add(INSTANCE._myTextureHeight);
    	return myResult;
    }
    //--------- MEMBER --------------------------------------------------------
    //-------------------------------------------------------------------------
	private CameraTexture(Activity activity) {
		if (theActivity == null || theActivity != activity) {
			theActivity=activity;
			theSurfaceView = new SurfaceView(theActivity);
			theActivity.addContentView(theSurfaceView, new LayoutParams(1,1));
		}
	}
	private CameraTexture() {} // hide constructor
	//-------------------------------------------------------------------------
	private void start() {
		if(INSTANCE != null) close(); 
		if(gl == null) return; 
		if(_myTextureID==0) {
			_myTextureID = createTextureID();
		}
		_mySurfaceHolder = theSurfaceView.getHolder();
		_mySurfaceHolder.setType( SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS );
        _mySurfaceHolder.addCallback( this );
        _myCamera = Camera.open();
        Parameters params = _myCamera.getParameters();
        params.setRotation(90);
        _myCamera.setParameters(params);
		if ( _myCamera == null ) { 
	        AC_Log.print( "CameraTexture: Camera is null" ); 
	        return; 
	    }
	    try { 
        	_myCamera.setPreviewDisplay( _mySurfaceHolder );
        } catch ( Throwable t ) { 
        	AC_Log.print( "CameraTexture: Exception in setPreviewDisplay()");
        }
    	_myCamWidth = _myCamera.getParameters().getPreviewSize().width;
		_myCamHeight= _myCamera.getParameters().getPreviewSize().height;
		_myTextureWidth = (int) Math.pow( 2,(int)( Math.log(_myCamWidth) / Math.log(2) ) + 1);
		_myTextureHeight = (int) Math.pow( 2, (int)(Math.log(_myCamHeight) / Math.log(2) ) + 1);
		_myCamera.setPreviewCallback( new PreviewCallback() {
                // preview callback:
				public void onPreviewFrame( byte[] data, Camera camera ) {
                	_myCamData=data;
                	_newFrameAvailable = true;
                }});
		 _myCamera.startPreview();
    }
	//-------------------------------------------------------------------------
    public void bind() {
    	if (!_newFrameAvailable) return; // only bind if new picture was token 
   		_newFrameAvailable = false;

   		byte myRGBCamData[] = new byte[(int) (_myCamWidth * _myCamHeight * 2.0)];
   		
   		if (_myColorConversionFlag) {
   			toRGB565(_myCamData, _myCamWidth, _myCamHeight, myRGBCamData);
   		}
   		gl.glBindTexture(GL10.GL_TEXTURE_2D, _myTextureID);
   		if (_myColorConversionFlag) {
   	   		gl.glTexImage2D(GL10.GL_TEXTURE_2D, 0, GL10.GL_RGB, _myTextureWidth, _myTextureHeight, 
   					0, GL10.GL_RGB, GL10.GL_UNSIGNED_SHORT_5_6_5, null);
	   		gl.glTexSubImage2D(GL10.GL_TEXTURE_2D, 0, 0, 0, _myCamWidth, _myCamHeight, GL10.GL_RGB, 
							   GL10.GL_UNSIGNED_SHORT_5_6_5, ByteBuffer.wrap(myRGBCamData));
   		} else {
   	   		gl.glTexImage2D(GL10.GL_TEXTURE_2D, 0, GL10.GL_LUMINANCE, _myTextureWidth, _myTextureHeight, 
   					0, GL10.GL_LUMINANCE, GL10.GL_UNSIGNED_BYTE, null);
	   		gl.glTexSubImage2D(GL10.GL_TEXTURE_2D, 0, 0, 0, _myCamWidth, _myCamHeight, GL10.GL_LUMINANCE, 
					   GL10.GL_UNSIGNED_BYTE, ByteBuffer.wrap(_myCamData));
   		}
		gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
    }
    
    private void toRGB565(byte[] yuvs, int width, int height, byte[] rgbs) {
        //the end of the luminance data
        final int lumEnd = width * height;
        //points to the next luminance value pair
        int lumPtr = 0;
        //points to the next chromiance value pair
        int chrPtr = lumEnd;
        //points to the next byte output pair of RGB565 value
        int outPtr = 0;
        //the end of the current luminance scanline
        int lineEnd = width;

        while (true) {

            //skip back to the start of the chromiance values when necessary
            if (lumPtr == lineEnd) {
                if (lumPtr == lumEnd) break; //we've reached the end
                //division here is a bit expensive, but's only done once per scanline
                chrPtr = lumEnd + ((lumPtr  >> 1) / width) * width;
                lineEnd += width;
            }

            //read the luminance and chromiance values
            final int Y1 = yuvs[lumPtr++] & 0xff; 
            final int Y2 = yuvs[lumPtr++] & 0xff; 
            final int Cr = (yuvs[chrPtr++] & 0xff) - 128; 
            final int Cb = (yuvs[chrPtr++] & 0xff) - 128;
            int R, G, B;

            //generate first RGB components
            B = Y1 + ((454 * Cb) >> 8);
            if(B < 0) B = 0; else if(B > 255) B = 255; 
            G = Y1 - ((88 * Cb + 183 * Cr) >> 8); 
            if(G < 0) G = 0; else if(G > 255) G = 255; 
            R = Y1 + ((359 * Cr) >> 8); 
            if(R < 0) R = 0; else if(R > 255) R = 255; 
            //NOTE: this assume little-endian encoding
            rgbs[outPtr++]  = (byte) (((G & 0x3c) << 3) | (B >> 3));
            rgbs[outPtr++]  = (byte) ((R & 0xf8) | (G >> 5));

            //generate second RGB components
            B = Y2 + ((454 * Cb) >> 8);
            if(B < 0) B = 0; else if(B > 255) B = 255; 
            G = Y2 - ((88 * Cb + 183 * Cr) >> 8); 
            if(G < 0) G = 0; else if(G > 255) G = 255; 
            R = Y2 + ((359 * Cr) >> 8); 
            if(R < 0) R = 0; else if(R > 255) R = 255; 
            //NOTE: this assume little-endian encoding
            rgbs[outPtr++]  = (byte) (((G & 0x3c) << 3) | (B >> 3));
            rgbs[outPtr++]  = (byte) ((R & 0xf8) | (G >> 5));
        }
    }    
	//-------------------------------------------------------------------------
	private int createTextureID() {
		if (_myCameraTextures==null)
			_myCameraTextures=new int[1];
		else
			gl.glDeleteTextures(1, _myCameraTextures, 0);
		gl.glGenTextures(1, _myCameraTextures, 0);
		return _myTextureID = _myCameraTextures[0];
	}
	//-------------------------------------------------------------------------
	public void close() {
		if ( _myCamera != null ) {
			_myCamera.setPreviewCallback(null);
			_myCamera.stopPreview();
			_myCamera.release();
			_myCamera = null;
        }
    	if (_mySurfaceHolder !=null) {
    		_mySurfaceHolder.removeCallback( INSTANCE );
    		_mySurfaceHolder=null;
    	}
	}
	//-------------------------------------------------------------------------
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) { }
	//-------------------------------------------------------------------------
    public void surfaceCreated(SurfaceHolder holder) { /*startCamera();*/ }
	//-------------------------------------------------------------------------
  	public void surfaceDestroyed(SurfaceHolder holder) { closeCamera(); }
} //end
