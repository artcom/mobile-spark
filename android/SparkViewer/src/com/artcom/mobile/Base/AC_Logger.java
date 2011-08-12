package com.artcom.mobile.Base;

public class AC_Logger {
	public static void print(String theMessage) {
		NativeBinding.log(0, new Throwable().getStackTrace()[1].getFileName(), 
	    		             new Throwable().getStackTrace()[1].getLineNumber(), 
	    		             "SparkViewer created, ready to call native [cpp logger].");   
	}

}
