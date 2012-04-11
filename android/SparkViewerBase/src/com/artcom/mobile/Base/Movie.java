// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.Base;

import java.util.List;
import java.util.ArrayList;
import java.io.IOException;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.opengl.GLES20;
import android.view.Surface;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;

public class Movie implements SurfaceTexture.OnFrameAvailableListener {

    private MediaPlayer _myMediaPlayer;
    private int[] _myTextureId;
    private SurfaceTexture _mySurfaceTexture;
    private String _myCurrentPath = "";
    private boolean _myUpdateSurfaceFlag = false;
    private static int GL_TEXTURE_EXTERNAL_OES = 0x8D65;

    public Movie() {
        init();
    }

    protected void finalize ()  {
        AC_Log.debug("Movie::DTOR");
        release();
    }

    private void release() {
        AC_Log.debug("Movie::release");
        if (_myMediaPlayer != null){
            _myMediaPlayer.release();
        }
        if (_myTextureId[0] != 0) {
            GLES20.glDeleteTextures(1, _myTextureId, 0);
        }
    }

    private void init() {
        AC_Log.debug("Movie::init");
        int[] textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);

        _myTextureId = textures;
        GLES20.glBindTexture(GL_TEXTURE_EXTERNAL_OES, _myTextureId[0]);

        GLES20.glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER,
                GLES20.GL_NEAREST);
        GLES20.glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER,
                GLES20.GL_LINEAR);

        _mySurfaceTexture = new SurfaceTexture(_myTextureId[0]);
        _mySurfaceTexture.setOnFrameAvailableListener(this);

        Surface mySurface = new Surface(_mySurfaceTexture);
        _myMediaPlayer = new MediaPlayer();
        _myMediaPlayer.setSurface(mySurface);
        mySurface.release();
        // TODO: switch to async
        //_myMediaPlayer.setOnPreparedListener(this);
    }

    // Called when MediaPlayer is ready
    public void onPrepared(MediaPlayer player) {
        //player.start();

    }

    synchronized public void onFrameAvailable(SurfaceTexture surface) {
        _myUpdateSurfaceFlag = true;
    }

    public void load(String thePath) {
        AC_Log.debug("Movie::load " + thePath);
        if (isPlaying()) {
            _myMediaPlayer.stop();
        }
        if (!("".equals(_myCurrentPath)) && ! _myCurrentPath.equals(thePath)){
            release();
            init();
        }
        try {
            if (!_myCurrentPath.equals(thePath)) {
                _myMediaPlayer.setDataSource(thePath);
                _myMediaPlayer.prepare();
            }
            _myCurrentPath = thePath;
        } catch (IOException e){
            AC_Log.error("mediaplayer setDataSource failed");
        }
    }

    public void play() {
        _myMediaPlayer.start();
    }

    public void stop() {
        _myMediaPlayer.stop();
        release();
    }

    public void pause() {
        _myMediaPlayer.pause();
    }

    public void updateMovieTexture() {
        if (_myUpdateSurfaceFlag) {
            _mySurfaceTexture.updateTexImage();
            _myUpdateSurfaceFlag = false;
        }
    }

    public List<Integer> getMovieInfo() {
        List<Integer> myResult = new ArrayList<Integer>();
        myResult.add(_myTextureId[0]);
        myResult.add(_myMediaPlayer.getVideoWidth());
        myResult.add(_myMediaPlayer.getVideoHeight());
        return myResult;
    }

    public boolean isPlaying() {
        if (_myMediaPlayer != null) {
            return _myMediaPlayer.isPlaying();
        } else {
            return false;
        }
    }

    public void setVolume(float theVolume) {
        _myMediaPlayer.setVolume(theVolume, theVolume);
    }

}

