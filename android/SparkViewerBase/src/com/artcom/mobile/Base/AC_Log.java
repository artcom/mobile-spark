package com.artcom.mobile.Base;

import com.artcom.mobile.Base.Severity;

public class AC_Log {
    public static void setTopLevelTag(String theTagString) {
        NativeBinding.setLoggerTopLevelTag(theTagString);
    }

    public static void setSeverity(Severity theSeverity) {
        NativeBinding.setSeverity(theSeverity);
        _myGlobalSeverity = theSeverity;
    }

    private static void log(Severity theSeverity, String theMessage) {
        NativeBinding.log(theSeverity, new Throwable().getStackTrace()[2].getFileName(), 
                         new Throwable().getStackTrace()[2].getLineNumber(), 
                         theMessage);   
    }
    public static void info(String theMessage) {
        if (Severity.SEV_INFO.compareTo(_myGlobalSeverity) < 0) {
            log(Severity.SEV_INFO, theMessage);
        }
    }
    public static void error(String theMessage) {
        if (Severity.SEV_ERROR.compareTo(_myGlobalSeverity) < 0) {
            log(Severity.SEV_ERROR, theMessage);
        }
    }
    public static void print(String theMessage) {
        if (Severity.SEV_PRINT.compareTo(_myGlobalSeverity) < 0) {
            log(Severity.SEV_PRINT, theMessage);
        }
    }
    public static void warn(String theMessage) {
        if (Severity.SEV_WARNING.compareTo(_myGlobalSeverity) < 0) {
            log(Severity.SEV_WARNING, theMessage);
        }
    }
    public static void debug(String theMessage) {
        if (Severity.SEV_DEBUG.compareTo(_myGlobalSeverity) < 0) {
            log(Severity.SEV_DEBUG, theMessage);
        }
    }
    public static void trace(String theMessage) {
        if (Severity.SEV_TRACE.compareTo(_myGlobalSeverity) < 0) {
            log(Severity.SEV_TRACE, theMessage);
        }
    }
    private static Severity _myGlobalSeverity = Severity.SEV_WARNING;
}
