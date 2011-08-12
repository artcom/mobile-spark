package com.artcom.mobile.test;

import com.artcom.mobile.Base.*;
import com.artcom.mobile.test.TestBaseBinding;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.test.AndroidTestCase;
import android.util.Log;
import junit.framework.Assert;

public class BaseNativeLibTest extends AndroidTestCase {
    private static String LOG_TAG = "TestResult";
    private static String PACKAGE_NAME = "com.artcom.mobile.test";
       
	public void testLibTest() throws Throwable {
		System.out.println(System.getProperty("java.library.path"));
		Log.i(LOG_TAG, System.getProperty("java.library.path"));

		String myResult = TestBaseBinding.test(); 
	    Assert.assertEquals("\n" + myResult + "\n", myResult.length(), 0);// run libs own tests

	    
		myResult = TestAndroidBinding.test(getApkFilePath()); 
	    Assert.assertEquals("\n" + myResult + "\n", myResult.length(), 0);// run libs own tests
    }
	
	public void testJavaTest() throws Throwable {		
		 // test java stuff
	    Assert.assertTrue(true);
	    
	    
	    
    }
	
	
	   private String getApkFilePath() {
           Context context = getContext();
           ApplicationInfo appInfo = null;
           String apkFilePath = null;
           PackageManager packMgmr = context.getPackageManager();
           try {
               appInfo = packMgmr.getApplicationInfo(PACKAGE_NAME, 0);
           } catch (NameNotFoundException e) {
               e.printStackTrace();
               throw new RuntimeException("Unable to locate assets, aborting...");
           }
           apkFilePath = appInfo.sourceDir;
           return apkFilePath;
       }      
}

   
