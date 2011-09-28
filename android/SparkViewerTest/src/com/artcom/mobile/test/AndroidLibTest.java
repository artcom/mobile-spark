package com.artcom.mobile.test;

import junit.framework.Assert;

import com.artcom.mobile.Base.APK;
import com.artcom.mobile.test.AndroidBinding;

import android.util.Log;

import android.test.AndroidTestCase;

public class AndroidLibTest  extends AndroidTestCase{
    private static String PACKAGE_NAME = "com.artcom.mobile.test";

    public void testLib() throws Throwable {
        // run libs tests method
        String myResult = AndroidBinding.test(APK.getApkFilePath(PACKAGE_NAME, getContext()));
        Assert.assertEquals("\n" + myResult + "\n", myResult.length(), 0);
    }

    public void testLibsInterface() throws Throwable {
         // test bound methods
   }
    public void testJava() throws Throwable {
         // test java stuff
        Log.v("Test", PACKAGE_NAME);
        Log.v("Test","......." + APK.getApkFilePath(PACKAGE_NAME, getContext()));
        Assert.assertTrue(APK.getApkFilePath(PACKAGE_NAME, getContext()).length()>0);
   }

}
