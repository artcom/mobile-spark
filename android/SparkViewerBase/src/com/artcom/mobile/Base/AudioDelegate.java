package com.artcom.mobile.Base;

import android.app.Activity;

public class AudioDelegate {

	private static Sound soundPlayer;
    private static Music backgroundMusicPlayer;

    
    public AudioDelegate(Activity theActivity) {
    	soundPlayer = new Sound(theActivity);
        backgroundMusicPlayer = new Music(theActivity);
	}
    
    
///////////////////////////////static audio functions called from c++  
    public static void playBackgroundMusic(String path, boolean isLoop){
        backgroundMusicPlayer.playBackgroundMusic(path, isLoop);
    }
    public static void stopBackgroundMusic(){
        backgroundMusicPlayer.stopBackgroundMusic();
    }
    public static void pauseBackgroundMusic(){
        backgroundMusicPlayer.pauseBackgroundMusic();
    }
    public static void resumeBackgroundMusic(){
        backgroundMusicPlayer.resumeBackgroundMusic();
    }
    public static void rewindBackgroundMusic(){
        backgroundMusicPlayer.rewindBackgroundMusic();
    }
    public static boolean isBackgroundMusicPlaying(){
        return backgroundMusicPlayer.isBackgroundMusicPlaying();
    }
    public static float getBackgroundMusicVolume(){
        return backgroundMusicPlayer.getBackgroundVolume();
    }
    public static void setBackgroundMusicVolume(float volume){
        backgroundMusicPlayer.setBackgroundVolume(volume);
    }
    public static int playEffect(String path){
        return soundPlayer.playEffect(path);
    }
    public static void stopEffect(int soundId){
        soundPlayer.stopEffect(soundId);
    }
    public static float getEffectsVolume(){
        return soundPlayer.getEffectsVolume();
    }
    public static void setEffectsVolume(float volume){
        soundPlayer.setEffectsVolume(volume);
    }
    public static void preloadEffect(String path){
        soundPlayer.preloadEffect(path);
    }
    public static void unloadEffect(String path){
        soundPlayer.unloadEffect(path);
    }
    public static void end(){
        backgroundMusicPlayer.end();
        soundPlayer.end();
    }
}
