package com.artcom.mobile.Base;

import android.os.Bundle;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class EnvMap {
   private Map<String,String> _myEnvironmentVariables = new HashMap<String, String>(); 

   public void EnvMap () {
   }

   public void init (Bundle theExtras) {
        if ( theExtras != null ) {
            String myEnvVar = theExtras.getString("env_0");
            for (int i = 1; myEnvVar != null; ++i) {
                String[] myKeyValue = myEnvVar.split("=");
                String myKey = (myKeyValue.length > 0) ? myKeyValue[0] : "";
                String myValue = (myKeyValue.length > 1) ? myKeyValue[1] : "";
                AC_Log.info("found env " + myKey + " , " + myValue);
                if (myValue != "") {
                    _myEnvironmentVariables.put(myKey, myValue);
                    NativeBinding.putEnv(myEnvVar);
                }
                myEnvVar = theExtras.getString("env_" + i);
            }
        }
   }
    
   public boolean hasEnv(String theKey) {
       return _myEnvironmentVariables.containsKey(theKey);
   }
   public String getEnv(String theKey) {
       if (_myEnvironmentVariables.containsKey(theKey)) {
           return _myEnvironmentVariables.get(theKey);
       }
       AC_Log.error("environment variable '" + theKey + "' not found");
       return "";
   }
}

