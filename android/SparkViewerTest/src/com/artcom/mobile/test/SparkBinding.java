// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.test;

public class SparkBinding {
    static {
        System.loadLibrary("masl");
        System.loadLibrary("animation");
        System.loadLibrary("mar");
        System.loadLibrary("android");
        System.loadLibrary("spark");
        System.loadLibrary("testSpark");
    }
    public static native String test();
}

