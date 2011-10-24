// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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


