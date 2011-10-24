// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.Base;

public class NativeException extends Exception {


    String what_;
    String where_;

    public NativeException() {
        super();
    }

    public NativeException(String what) {
        super(what);
        what_ = what;
    }

    public NativeException(String what, String where) {
        super(what);
        what_ = what;
        where_ = where;
    }
}
