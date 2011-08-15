package com.artcom.mobile.Base;

public class AC_Log {
	public static void setTopLevelTag(String theTagString) {
		NativeBinding.setLoggerTopLevelTag(theTagString);
	}

	public static void error(String theMessage) {
		NativeBinding.log(0, new Throwable().getStackTrace()[1].getFileName(), 
	    		             new Throwable().getStackTrace()[1].getLineNumber(), 
	    		             theMessage);   
	}
	public static void print(String theMessage) {
		NativeBinding.log(0, new Throwable().getStackTrace()[1].getFileName(), 
	    		             new Throwable().getStackTrace()[1].getLineNumber(), 
	    		             theMessage);   
	}
	public static void warn(String theMessage) {
		NativeBinding.log(0, new Throwable().getStackTrace()[1].getFileName(), 
	    		             new Throwable().getStackTrace()[1].getLineNumber(), 
	    		             theMessage);   
	}
	
}
