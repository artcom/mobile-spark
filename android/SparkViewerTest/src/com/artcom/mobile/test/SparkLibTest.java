// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.test;

import junit.framework.Assert;
import android.test.AndroidTestCase;
import android.util.Log;


public class SparkLibTest extends AndroidTestCase{

    public static String LOG_TAG = "SparkLibTest";

    public void testLib() throws Throwable {
        String myResult = SparkBinding.test();
        Assert.assertEquals("\n" + myResult + "\n", myResult.length(), 0);
    }

    public void testLibsInterface() throws Throwable {
         // test bound methods
   }
}

