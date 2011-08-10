package com.artcom.mobile.test;

import junit.framework.Test;
import junit.framework.TestSuite;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.test.suitebuilder.TestSuiteBuilder;

public class AllTests  extends TestSuite{
	   public static Test suite() {
	        return new TestSuiteBuilder(AllTests.class).includeAllPackagesUnderHere().build();
	   }
	
}
