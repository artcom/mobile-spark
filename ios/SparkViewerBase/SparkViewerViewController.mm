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

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration 
{
    m_orientation = toInterfaceOrientation;
    
    CGRect newBounds = self.view.bounds;
    
    float smallerSide = std::min(newBounds.size.width, newBounds.size.height);
    float largerSide = std::max(newBounds.size.width, newBounds.size.height);
    
    bool isLandscape = UIInterfaceOrientationIsLandscape(toInterfaceOrientation);
    
    newBounds.size.width = isLandscape ? largerSide : smallerSide;
    newBounds.size.height = isLandscape ? smallerSide : largerSide;
    
    [(GLView*)self.view resizeView:newBounds];
}

-(void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation 
{
    //AC_PRINT<<"w: "<<self.view.bounds.size.width<<" -- h: "<<self.view.bounds.size.height;
    //AC_PRINT<<[[UIScreen mainScreen] scale];
    //[(GLView*)self.view onResize:self.view.bounds];
    
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{   
    std::string orientationString = masl::MobileSDK_Singleton::get().getNative()->getOrientation();
    
    bool ret = true;
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) 
    {
        ret =  (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } 
    
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