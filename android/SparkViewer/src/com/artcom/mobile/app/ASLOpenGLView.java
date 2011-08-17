package com.artcom.mobile.app;

import com.artcom.mobile.Base.*; 
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;   //needed only for method signaturs

import com.artcom.mobile.Base.APK;
import com.artcom.mobile.Base.AC_Log;

public class ASLOpenGLView extends GLSurfaceView {
    
    public static final String PACKAGE_NAME = "com.artcom.mobile";
    private static String LOG_TAG = "ASLOpenGLView";
    
    public static final String LAYOUT_FILE = "assets/layouts/main.spark";
    
    private Play3DRenderer myRenderer;

    public ASLOpenGLView(Context context) {
        super(context);
        myRenderer = new Play3DRenderer(context);
        init(false, 0, 0);
    }

    public ASLOpenGLView(Context context, boolean translucent, int depth, int stencil) {
        super(context);
        init(translucent, depth, stencil);
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
    	AC_Log.print("View.onTouchEvent");
        NativeBinding.onTouch();
        return super.onTouchEvent(event);
    }
    
    private void init(boolean translucent, int depth, int stencil) {

        /* By default, GLSurfaceView() creates a RGB_565 opaque surface.
         * If we want a translucent one, we should change the surface's
         * format here, using PixelFormat.TRANSLUCENT for GL Surfaces
         * is interpreted as any 32-bit surface with alpha by SurfaceFlinger.
         */
        if (translucent) {
            this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        }
        setEGLConfigChooser(new AndroidEGLConfigChooser(AndroidEGLConfigChooser.ConfigType.BEST, false));
        setEGLContextFactory(new ContextFactory());
        setRenderer(myRenderer);
    }

    private static class ContextFactory implements GLSurfaceView.EGLContextFactory {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
            AC_Log.warn("creating OpenGL ES 2.0 context");
            checkEglError("Before eglCreateContext", egl);
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
            EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
            checkEglError("After eglCreateContext", egl);
            return context;
        }

        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
            egl.eglDestroyContext(display, context);
        }
    }

    private static void checkEglError(String prompt, EGL10 egl) {
        int error;
        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
        	AC_Log.error(String.format("%s: EGL error: 0x%x", prompt, error));
        }
    }

    private static class Play3DRenderer implements GLSurfaceView.Renderer {
        
        public static int numFrames = 0;
        public static long millisec = 0;

        private Context context;
        public Play3DRenderer (Context context) {
            this.context = context;
           
        }
        
        public void onDrawFrame(GL10 glUnused) {
            updateFrameCounter();
            NativeBinding.onFrame(System.currentTimeMillis());
        }

        public void onSurfaceChanged(GL10 glUnused, int width, int height) {
        	
        }
        
        private void updateFrameCounter() {
            if (numFrames == 0) {
                millisec = System.currentTimeMillis();
            } else if (numFrames == 99) {
                long now = System.currentTimeMillis();
                AC_Log.print("num Frames " + numFrames);
                AC_Log.print("time " + (now- millisec));
                float fps = (float)numFrames/(float)(now-millisec) * 1000.0f;
                AC_Log.print("fps " + fps);
                millisec = now;
                numFrames = 0;
            }
            numFrames++;
        }

        public void onSurfaceCreated(GL10 glUnused, EGLConfig config) {
        	AC_Log.print("_________________________________- on surface created");
            NativeBinding.setup(APK.getApkFilePath(PACKAGE_NAME, context), LAYOUT_FILE);
        }
    }
}

