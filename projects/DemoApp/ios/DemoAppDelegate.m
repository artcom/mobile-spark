// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#import "DemoAppDelegate.h"
#import "DemoAppGLView.h"

@implementation DemoAppDelegate

- (void)createGLView
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];    
    [self initialize:@"DemoApp"];

    // this call will also create the Spark-Application ...
    myGLView = [[DemoAppGLView alloc]initWithFrame:[[UIScreen mainScreen] bounds]] ;
}

- (void)dealloc
{
    [super dealloc];
}

@end
