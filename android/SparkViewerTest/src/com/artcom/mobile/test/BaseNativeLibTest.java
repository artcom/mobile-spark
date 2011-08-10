package com.artcom.mobile.test;

import com.artcom.mobile.BaseNativeLib;
import com.artcom.mobile.test.TestBaseLib;

import android.test.AndroidTestCase;
import android.util.Log;
import junit.framework.Assert;

public class BaseNativeLibTest extends AndroidTestCase{
    private static String LOG_TAG = "TestResult";
	
	public void testLibTest() throws Throwable {
		System.out.println(System.getProperty("java.library.path"));
		Log.i(LOG_TAG, System.getProperty("java.library.path"));
		String myResult = TestBaseLib.test(); 
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
