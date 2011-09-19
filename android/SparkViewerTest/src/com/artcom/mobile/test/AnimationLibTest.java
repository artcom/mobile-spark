package com.artcom.mobile.test;

import junit.framework.Assert;
import android.test.AndroidTestCase;
import android.util.Log;


public class AnimationLibTest extends AndroidTestCase{

    public static String LOG_TAG = "AnimationLibTest";

    public void testLib() throws Throwable {
        String myResult = AnimationBinding.test();
        Assert.assertEquals("\n" + myResult + "\n", myResult.length(), 0);
    }

    public void testLibsInterface() throws Throwable {
         // test bound methods
   }
}
