package com.artcom.mobile.Base;

public enum Severity {
    SEV_PRINT("PRINT"),
    SEV_TESTRESULT("TESTRESULT"),
    SEV_FATAL("FATAL"),
    SEV_ERROR("ERROR"),
    SEV_WARNING("WARNING"),
    SEV_INFO("INFO"),
    SEV_DEBUG("DEBUG"),
    SEV_TRACE("TRACE"),
    SEV_DISABLED("DISABLED");

    private String text;

    Severity(String text) {
        this.text = text;
    }

    public static Severity fromString(String theText) {
        if (theText != null) {
            for (Severity s : Severity.values()) {
                if (theText.equalsIgnoreCase(s.text)) {
                    return s;
                }
            }
        }
        AC_Log.error("Severity " + theText + " doesn't exist returning default SEV_WARNING");
        return SEV_WARNING;
    }
}


