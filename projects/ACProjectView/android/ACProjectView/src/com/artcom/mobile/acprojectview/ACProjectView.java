// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

package com.artcom.mobile.acprojectview;

import android.os.Bundle;
import com.artcom.mobile.Base.NativeBinding;
import com.artcom.mobile.Base.SparkViewerActivity;


public class ACProjectView extends SparkViewerActivity {
    static {
        NativeBinding.loadLibraries();
        System.loadLibrary("ACProjectView"); 
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        LOG_TAG = "ACProjectView";
        _myPackageExtension = ".acprojectview";
        super.onCreate(savedInstanceState);
    }

}
