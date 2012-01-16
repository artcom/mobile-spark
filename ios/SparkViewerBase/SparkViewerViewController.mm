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

#include "iOS/IOSMobileSDK.h"
#include <spark/Window.h>

@implementation SparkViewerViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nil bundle:nil];
    self.view.contentMode = UIViewContentModeLeft;
    
    return self;
}



-(void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{   
    std::string orientationString = masl::MobileSDK_Singleton::get().getNative()->getOrientation();
    
    bool ret = interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown;
    
    if(orientationString == spark::Orientation::PORTRAIT)
    {
        ret = interfaceOrientation == UIInterfaceOrientationPortrait;
    }
    else if (orientationString == spark::Orientation::LANDSCAPE)
    {
        ret =   interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
                interfaceOrientation == UIInterfaceOrientationLandscapeRight;
    }
    
    return ret;
}

-(void)dealloc {
    [super dealloc];
}

@end