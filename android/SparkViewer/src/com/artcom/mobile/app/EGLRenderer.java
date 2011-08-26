package com.artcom.mobile.app;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;

import com.artcom.mobile.Base.AC_Log;
import com.artcom.mobile.Base.APK;
import com.artcom.mobile.Base.NativeBinding;

public class EGLRenderer  implements GLSurfaceView.Renderer{
    public static int numFrames = 0;
    public static long millisec = 0;
	public static final String PACKAGE_NAME = "com.artcom.mobile";
	public static final String LAYOUT_FILE = "assets/layouts/main.spark";        
    public static boolean _myFirstTimeFlag;
    private Context context;
    public EGLRenderer (Context context, boolean firstTime) {
    	_myFirstTimeFlag = firstTime;
        this.context = context;
       
    }    
    public void onDrawFrame(GL10 glUnused) {
        updateFrameCounter();
        NativeBinding.onFrame(System.currentTimeMillis());
    }

    public void onSurfaceChanged(GL10 glUnused, int width, int height) {    	
    	NativeBinding.onSizeChanged(width,height);    
    }
    
    private void updateFrameCounter() {
        if (numFrames == 0) {
            millisec = System.currentTimeMillis();
        } else if (numFrames == 99) {
            long now = System.currentTimeMillis();
            //AC_Log.print("num Frames " + numFrames);
            //AC_Log.print("time " + (now- millisec));
            float fps = (float)numFrames/(float)(now-millisec) * 1000.0f;
            AC_Log.print("fps " + fps);
            millisec = now;
            numFrames = 0;
        }
        numFrames++;
    }


    public void onSurfaceCreated(GL10 glUnused, EGLConfig config) {
    	AC_Log.print("_________________________________- on surface created");
    	if (_myFirstTimeFlag) {
    		NativeBinding.setup(System.currentTimeMillis(), APK.getApkFilePath(PACKAGE_NAME, context), LAYOUT_FILE);
    	} else {
    		NativeBinding.onResume();
    	}
		
    }

}
