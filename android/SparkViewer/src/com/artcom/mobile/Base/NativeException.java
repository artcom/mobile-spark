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
