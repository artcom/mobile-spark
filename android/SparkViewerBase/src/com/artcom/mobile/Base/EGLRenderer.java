// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.Base;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.DisplayMetrics;


public class EGLRenderer  implements GLSurfaceView.Renderer{
    //public static int numFrames = 0;
    private static int _myScreenWidth;
    private static int _myScreenHeight;
    private static String _myPackageName;
    private SparkOpenGLViewDelegate _myViewDelegate; 
    private Context context;
    
    public EGLRenderer (Context context, SparkOpenGLViewDelegate theViewDelegate, String thePackageName, int theScreenWidth, int theScreenHeight) {
        _myScreenWidth = theScreenWidth;
        _myScreenHeight = theScreenHeight;
        _myPackageName = thePackageName;
        _myViewDelegate = theViewDelegate;
        this.context = context;

    }
    public void onDrawFrame(GL10 glUnused) {
        String myEvent = "<StageEvent type='frame' time='" + System.currentTimeMillis() + "'/>";
        NativeBinding.onEvent(myEvent);
        NativeBinding.handleEvents();
    }

    public void onSurfaceChanged(GL10 glUnused, int width, int height) {
        AC_Log.print("_________________________________- onSurfaceChanged");
        String myEvent = "<WindowEvent type='on_resize' newsize='[" + width + "," + height + "]' oldsize='[-1, -1]'/>";
        NativeBinding.onEvent(myEvent);
    }

    public void onSurfaceCreated(GL10 glContext, EGLConfig config) {
        CameraTexture.initWithContext(glContext);
        AC_Log.print("_________________________________- on surface created of " + _myPackageName);
        NativeBinding.initBinding();
        AC_Log.print(String.format("Spark Loaded %b " , _myViewDelegate.sparkWorldIsLoaded));
        if (!_myViewDelegate.sparkWorldIsLoaded) {
            NativeBinding.setup(System.currentTimeMillis(), APK.getApkFilePath(_myPackageName, context), _myScreenWidth,  _myScreenHeight);
            _myViewDelegate.sparkWorldIsLoaded = true;
        } else {
            NativeBinding.onResume();
        }
    }

}
