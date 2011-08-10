package com.artcom.mobile.test;

import com.artcom.mobile.BaseNativeLib;

import android.test.AndroidTestCase;
import android.util.Log;
import junit.framework.Assert;

public class BaseNativeLibTest extends AndroidTestCase{
    private static String LOG_TAG = "TestResult2";
	
	public void testLibTest() throws Throwable {
		String myResult = BaseNativeLib.test(); 
		Log.w(LOG_TAG, myResult);
	    Assert.assertEquals("\n" + myResult + "\n", myResult.length(), 0);// run libs own tests
    }
	public void testLibsInterfaceTest() throws Throwable {		
		 // test bound methods
	    Assert.assertTrue(BaseNativeLib.loadSpark("/sdcard/test.spark"));
    }
	public void testJavaTest() throws Throwable {		
		 // test java stuff
	    Assert.assertTrue(true);
    }}
