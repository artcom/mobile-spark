package com.artcom.mobile.Base;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import javax.microedition.khronos.opengles.GL10;
import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;

public class CameraTexture implements SurfaceHolder.Callback {
	
    private static CameraTexture INSTANCE;
	private SurfaceView _mySurfaceView;
	private SurfaceHolder _mySurfaceHolder;
	private Camera _myCamera;
	private Activity _myActivity;
	private int _myCamWidth, _myCamHeight;
    private int _myTextureID;
	private int[] cameraTexture;
	private static byte camData[] = new byte[0];
	private GL10 gl;
	
	//--------- STATIC --------------------------------------------------------
	public static CameraTexture init(Activity activity) {
		AC_Log.print("static init _________________________");
		if(INSTANCE==null) INSTANCE = new CameraTexture(activity);
	    return INSTANCE;
	}
	public static void initWithContext(GL10 glContext) {
		if (INSTANCE == null) return;
		INSTANCE.gl=glContext;
	}
	//-------------------------------------------------------------------------
	public static void startCamera() {
		if (INSTANCE == null) return;
		INSTANCE.start();
	}
	//-------------------------------------------------------------------------
    public static void closeCamera() {
    	if (INSTANCE != null) INSTANCE.close();
    	INSTANCE=null;
    }
	//-------------------------------------------------------------------------
    public static void bindTexture() {
    	if (INSTANCE != null ) INSTANCE.bind();
    }
	//-------------------------------------------------------------------------
    // returns [0]-textureID; [1]-preview width; [2]-preview height
    public static List<Integer> getTextureParams() {
		if (INSTANCE == null ) return null;
		List<Integer> myResult = new ArrayList<Integer>();
		myResult.add(INSTANCE._myTextureID);
		myResult.add(INSTANCE._myCamWidth);
		myResult.add(INSTANCE._myCamHeight);		  
    	return myResult;
    }
    //--------- MEMBER --------------------------------------------------------
    //-------------------------------------------------------------------------
	private CameraTexture(Activity activity) {
		this._myActivity=activity;
		this._mySurfaceView = new SurfaceView(_myActivity);
		this._myActivity.addContentView(_mySurfaceView, new LayoutParams(1,1));
	}
	//-------------------------------------------------------------------------
	private void start() {
		if(INSTANCE != null)close(); 
		if(this.gl == null) return; 
		if(_myTextureID==0) {
			_myTextureID = createTextureID();
		}
		_mySurfaceHolder = _mySurfaceView.getHolder();
		_mySurfaceHolder.setType( SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS );
        _mySurfaceHolder.addCallback( this );
        _myCamera = Camera.open();
		if ( _myCamera == null ) { 
	        Log.e( "CameraTexture", "Camera is null" ); 
	        return; 
	    }
	    try { 
        	_myCamera.setPreviewDisplay( _mySurfaceHolder );
        } catch ( Throwable t ) { 
        	Log.e( "CameraTexture", "Exception in setPreviewDisplay()", t );
        }
    	_myCamWidth = _myCamera.getParameters().getPreviewSize().width;
		_myCamHeight= _myCamera.getParameters().getPreviewSize().height;
		camData = new byte[_myCamWidth * _myCamHeight * 3 / 2];
				_myCamera.setPreviewCallback( new PreviewCallback() {
                // preview callback:
				public void onPreviewFrame( byte[] data, Camera camera ) {
                	camData=data;
                }	
                	
                });
		 _myCamera.startPreview();
    }
	//-------------------------------------------------------------------------
    public void bind() {
    	gl.glBindTexture(GL10.GL_TEXTURE_2D, _myTextureID);
    	gl.glTexImage2D(GL10.GL_TEXTURE_2D, 0, GL10.GL_LUMINANCE, 1024, 512, 0, GL10.GL_LUMINANCE, GL10.GL_UNSIGNED_BYTE, null);
		gl.glTexSubImage2D(GL10.GL_TEXTURE_2D, 0, 0, 0, _myCamWidth, _myCamHeight, GL10.GL_LUMINANCE, GL10.GL_UNSIGNED_BYTE, ByteBuffer.wrap(camData));
		gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
    }
	//-------------------------------------------------------------------------
	private int createTextureID() {
		if (cameraTexture==null)
			cameraTexture=new int[1];
		else
			gl.glDeleteTextures(1, cameraTexture, 0);
		gl.glGenTextures(1, cameraTexture, 0);
		return _myTextureID = cameraTexture[0];
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
    public void surfaceCreated(SurfaceHolder holder) { start(); }
	//-------------------------------------------------------------------------
  	public void surfaceDestroyed(SurfaceHolder holder) { closeCamera(); }
} //end
