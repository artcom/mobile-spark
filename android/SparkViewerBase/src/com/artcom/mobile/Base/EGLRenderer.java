package com.artcom.mobile.Base;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.DisplayMetrics;


public class EGLRenderer  implements GLSurfaceView.Renderer{
    public static int numFrames = 0;
    public static long millisec = 0;
    public static final String PACKAGE_NAME = "com.artcom.mobile";
    public static boolean _myFirstTimeFlag;
    public static int _myScreenWidth;
    public static int _myScreenHeight;
    private Context context;    
    public EGLRenderer (Context context, int theScreenWidth, int theScreenHeight, boolean firstTime) {
        _myFirstTimeFlag = firstTime;
        _myScreenWidth = theScreenWidth;
        _myScreenHeight = theScreenHeight;
        this.context = context;
       
    }    
    public void onDrawFrame(GL10 glUnused) {
        updateFrameCounter();
        String myEvent = "<StageEvent type='frame' time='" + System.currentTimeMillis() + "'/>";
        NativeBinding.onEvent(myEvent);
    }

    public void onSurfaceChanged(GL10 glUnused, int width, int height) {        
        AC_Log.print("_________________________________- onSurfaceChanged");
        String myEvent = "<WindowEvent type='on_resize' newsize='[" + width + "," + height + "]' oldsize='[-1, -1]'/>";
        NativeBinding.onEvent(myEvent);        
    }
    
    private void updateFrameCounter() {
        if (numFrames == 0) {
            millisec = System.currentTimeMillis();
        } else if (numFrames == 99) {
            long now = System.currentTimeMillis();
            //AC_Log.print("num Frames " + numFrames);
            //AC_Log.print("time " + (now- millisec));
            float fps = (float)numFrames/(float)(now-millisec) * 1000.0f;
            //AC_Log.print("fps " + fps);
            millisec = now;
            numFrames = 0;
        }
        numFrames++;
    }


    public void onSurfaceCreated(GL10 glContext, EGLConfig config) {
    	CameraTexture.initWithContext(glContext);
        AC_Log.print("_________________________________- on surface created");
        NativeBinding.initBinding();
        if (_myFirstTimeFlag) {
            NativeBinding.setup(System.currentTimeMillis(), APK.getApkFilePath(PACKAGE_NAME, context), _myScreenWidth,  _myScreenHeight);
        } else {
            NativeBinding.onResume();
        }
    }

}
