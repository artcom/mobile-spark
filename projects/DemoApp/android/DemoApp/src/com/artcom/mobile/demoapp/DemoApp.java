package com.artcom.mobile.demoapp;

import android.os.Bundle;
import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;
import com.artcom.mobile.Base.Sound;

public class DemoApp extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("DemoApp"); 
    }
    private static Sound soundPlayer;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        LOG_TAG = "DemoApp";
        _myPackageExtension = ".demoapp";
        super.onCreate(savedInstanceState);
        soundPlayer = new Sound(this);
    }
    
    public static int playEffect(String path){
        return soundPlayer.playEffect(path);
    }    

}
