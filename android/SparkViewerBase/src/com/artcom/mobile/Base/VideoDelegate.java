package com.artcom.mobile.Base;

import java.util.List;

import android.app.Activity;

public class VideoDelegate {

    private static VideoPlayer _myVideoPlayer;
    
    public VideoDelegate(Activity theActivity) {
        _myVideoPlayer = new VideoPlayer(theActivity);
    }
    
    
    public static void playMovie(int theMovieId, String thePath) {
        _myVideoPlayer.playMovie(theMovieId, thePath);
    }
    public static void stopMovie(int theMovieId) {
        _myVideoPlayer.stopMovie(theMovieId);
    }
    public static void pauseMovie(int theMovieId) {
        _myVideoPlayer.pauseMovie(theMovieId);
    }
    public static void updateMovieTexture(int theMovieId) {
        _myVideoPlayer.updateMovieTexture(theMovieId);
    }
    public static List<Integer> getMovieInfo(int theMovieId) {
        return _myVideoPlayer.getMovieInfo(theMovieId);
    }
    public static boolean isMoviePlaying(int theMovieId) {
        return _myVideoPlayer.isMoviePlaying(theMovieId);
    }
    public static void setMovieVolume(int theMovieId, float theVolume) {
        _myVideoPlayer.setMovieVolume(theMovieId, theVolume);
    }
}
