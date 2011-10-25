// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "SparkViewerViewController.h"
#import "GLView.h"


@implementation SparkViewerViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nil bundle:nil];
    self.view.contentMode =UIViewContentModeLeft;
    return self;
}



-(void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{   
    if(isPortrait) return NO;
    return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

-(void)setPortrait:(bool)thePortrait {
    isPortrait = thePortrait;
    
}

-(void)dealloc {
    [super dealloc];
}

@end