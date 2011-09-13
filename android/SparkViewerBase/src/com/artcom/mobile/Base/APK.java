package com.artcom.mobile.Base;

//import com.artcom.mobile.BaseBinding;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

public class APK {

    public static String getApkFilePath(String thePackageName, Context theContext) {
        String apkFilePath = null;
        ApplicationInfo appInfo = null;
        PackageManager packMgmr = theContext.getPackageManager();
        try {
            appInfo = packMgmr.getApplicationInfo(thePackageName, 0);
        } catch (NameNotFoundException e) {
            e.printStackTrace();
            throw new RuntimeException("Unable to locate assets, aborting...");
        }
        apkFilePath = appInfo.sourceDir;
        return apkFilePath;
    }
}
