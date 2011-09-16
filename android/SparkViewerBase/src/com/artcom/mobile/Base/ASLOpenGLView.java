package com.artcom.mobile.Base;

import com.artcom.mobile.Base.*; 
import com.artcom.mobile.Base.AndroidEGLConfigChooser.ConfigType;

import android.content.Context;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;


public class ASLOpenGLView extends GLSurfaceView {
    
    
    private EGLRenderer myRenderer;

    public ASLOpenGLView(Context context, String thePackageExtension, int theScreenWidth, int theScreenHeight, boolean firstTime) {
        super(context);
        myRenderer = new EGLRenderer(context, thePackageExtension, theScreenWidth, theScreenHeight, firstTime);
        init();        
        setKeepScreenOn(true);   
    }

    public void onSizeChanged (int w, int h, int oldw, int oldh) {
        EventManager.onSizeChanged(w,h);
        String myEvent = "<WindowEvent type='on_resize' newsize='[" + w + "," + h + "]' oldsize='[" + oldw + "," + oldh + "]'/>";
        NativeBinding.onEvent(myEvent);        
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

