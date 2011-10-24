// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#import "ACProjectViewDelegate.h"
#import "ACProjectViewGLView.h"

@implementation ACProjectViewDelegate

- (void)createGLView {
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];    
    [self initialize:@"ACProjectView"];
    myGLView = [[ACProjectViewGLView alloc]initWithFrame: [self getWindowBoundsWithBaseLayout:@"/main"]];

}

- (void)dealloc
{
    [super dealloc];
}

@end
