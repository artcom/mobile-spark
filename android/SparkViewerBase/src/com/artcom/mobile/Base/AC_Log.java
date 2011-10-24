// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.Base;

import com.artcom.mobile.Base.Severity;

public class AC_Log {
    public static void setTopLevelTag(String theTagString) {
        NativeBinding.setLoggerTopLevelTag(theTagString);
    }

    public static void setSeverity(Severity theSeverity) {
        NativeBinding.setSeverity(theSeverity);
    }

    private static void log(Severity theSeverity, String theMessage) {
        NativeBinding.log(theSeverity, new Throwable().getStackTrace()[2].getFileName(),
                         new Throwable().getStackTrace()[2].getLineNumber(),
                         theMessage);
    }
    public static void info(String theMessage) {
        log(Severity.SEV_INFO, theMessage);
    }
    public static void error(String theMessage) {
        log(Severity.SEV_ERROR, theMessage);
    }
    public static void print(String theMessage) {
        log(Severity.SEV_PRINT, theMessage);
    }
    public static void warn(String theMessage) {
        log(Severity.SEV_WARNING, theMessage);
    }
    public static void debug(String theMessage) {
        log(Severity.SEV_DEBUG, theMessage);
    }
    public static void trace(String theMessage) {
        log(Severity.SEV_TRACE, theMessage);
    }
}
