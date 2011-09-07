package com.artcom.mobile.test;


import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.test.BaseBinding;
import android.test.AndroidTestCase;
import android.util.Log;
import junit.framework.Assert;

public class BaseLibTest extends AndroidTestCase {
    private static String LOG_TAG = "TestResult";
       
    public void testLib() throws Throwable {
        // run libs tests method        
        String myResult = BaseBinding.test(); 
        Assert.assertEquals("\n" + myResult + "\n", myResult.length(), 0);
        
    }
    public void testLibsInterface() throws Throwable {      
         // test bound methods
        Assert.assertTrue(NativeBinding.loadSpark("/sdcard/test.spark"));
    }
    public void testJava() throws Throwable {       
         // test java stuff
    }
    
    
}

   
