package com.artcom.mobile.Base;

import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import android.app.Activity;

public class VideoDelegate {

private static Map<Integer,Movie> _myMovies = new HashMap<Integer, Movie>();

public VideoDelegate(Activity theActivity) {
}


    public static void playMovie(int theMovieId, String thePath) {
        AC_Log.debug("VideoDelegate::playMovie " + thePath);
        if (_myMovies.containsKey(theMovieId)) {
             _myMovies.get(theMovieId).play(thePath);
        } else {
            Movie myMovie = new Movie();
            _myMovies.put(theMovieId, myMovie);
            myMovie.play(thePath);
        }
    }

    public static void stopMovie(int theMovieId) {
        if (_myMovies.containsKey(theMovieId)) {
            _myMovies.get(theMovieId).stop();
            _myMovies.remove(theMovieId);
        } else {
            //AC_Log.error("the movie " + theMovieId + " is not initialized yet");
        }
    }

    public static void pauseMovie(int theMovieId) {
        if (_myMovies.containsKey(theMovieId)) {
            _myMovies.get(theMovieId).pause();
        } else {
            //AC_Log.error("the movie " + theMovieId + " is not initialized yet");
        }
    }

    public static void updateMovieTexture(int theMovieId) {
        if (_myMovies.containsKey(theMovieId)) {
            _myMovies.get(theMovieId).updateMovieTexture();
        } else {
            //AC_Log.error("the movie " + theMovieId + " is not initialized yet");
        }
    }

    public static List<Integer> getMovieInfo(int theMovieId) {
        if (_myMovies.containsKey(theMovieId)) {
            return _myMovies.get(theMovieId).getMovieInfo();
        } else {
            //AC_Log.error("the movie " + theMovieId + " is not initialized yet");
            return new ArrayList<Integer>();
        }
    }

    public static boolean isMoviePlaying(int theMovieId) {
        AC_Log.debug("VideoDelegate::isMoviePlaying");
        if (_myMovies.containsKey(theMovieId)) {
            return _myMovies.get(theMovieId).isPlaying();
        } else {
            //AC_Log.error("the movie " + theMovieId + " is not initialized yet");
            return false;
        }
    }

    public static void setMovieVolume(int theMovieId, float theVolume) {
        if (_myMovies.containsKey(theMovieId)) {
            _myMovies.get(theMovieId).setVolume(theVolume);
        } else {
            //AC_Log.error("the movie " + theMovieId + " is not initialized yet");
        }
    }
}
