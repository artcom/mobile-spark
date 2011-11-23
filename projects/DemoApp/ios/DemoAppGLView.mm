// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#import "DemoAppGLView.h"

#include <spark/AppProvider.h>
#include <DemoApp.h>


@implementation DemoAppGLView

- (void) createApp
{
    spark::AppProvider::get().setApp(spark::BaseAppPtr(new demoapp::DemoApp()));
    //activate Multisample anti aliasing with useMSAA2x, useMSAA4x, useMSAA8x
    MSAAQuality = useNoneMSAA;
    
    //choose framerate. Default is 1 -> 60hz. 2 -> 30 hz. Evertything < 1 is undefined.  
    //frameInterval = 1;
}

- (void)dealloc
{
    [super dealloc];
}

@end
