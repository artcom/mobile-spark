package com.artcom.mobile.test;


import com.artcom.mobile.test.BaseBinding;
import android.test.AndroidTestCase;
import junit.framework.Assert;

public class BaseLibTest extends AndroidTestCase {

    public void testLib() throws Throwable {
        // run libs tests method
        String myResult = BaseBinding.test();
        Assert.assertEquals("\n" + myResult + "\n", myResult.length(), 0);

    }

    public void testJava() throws Throwable {
         // test java stuff
    }


}


