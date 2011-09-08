package com.artcom.mobile.app;

import com.artcom.mobile.Base.*; 
import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import com.artcom.mobile.Base.AC_Log;

public class ASLOpenGLView extends GLSurfaceView {
    
    
    private EGLRenderer myRenderer;
    private int width;
    private int height;

    public ASLOpenGLView(Context context, boolean firstTime) {
        super(context);
        myRenderer = new EGLRenderer(context, firstTime);
        init();        
        setKeepScreenOn(true);        
    }

    @Override
    public void onSizeChanged (int w, int h, int oldw, int oldh) {
        NativeBinding.onSizeChanged(w,h);
        width = w;
        height = h;
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        AC_Log.print("View.onTouchEvent");
        String myEvent = "<TouchEvent type='tap' x='" + event.getX() + "' y='" + (height - event.getY()) + "'/>";
        NativeBinding.onEvent(myEvent);
        return super.onTouchEvent(event);
    }
    
    private void init() {

        /* By default, GLSurfaceView() creates a RGB_565 opaque surface.
         * If we want a translucent one, we should change the surface's
         * format here, using PixelFormat.TRANSLUCENT for GL Surfaces
         * is interpreted as any 32-bit surface with alpha by SurfaceFlinger.
         */
        //this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
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
}

