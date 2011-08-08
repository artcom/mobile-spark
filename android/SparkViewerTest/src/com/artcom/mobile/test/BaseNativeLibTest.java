package com.artcom.mobile.test;

import com.artcom.mobile.BaseNativeLib;

import android.test.AndroidTestCase;
import junit.framework.Assert;

public class BaseNativeLibTest extends AndroidTestCase{
	public void testLibTest() throws Throwable {
	    Assert.assertTrue(BaseNativeLib.test());// run libs own tests
    }
	public void testLibsInterfaceTest() throws Throwable {		
		 // test bound methods
	    Assert.assertTrue(BaseNativeLib.loadSpark("/sdcard/test.spark"));
    }
	public void testJavaTest() throws Throwable {		
		 // test java stuff
	    Assert.assertTrue(true);
    }}
