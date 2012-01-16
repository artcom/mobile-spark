// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#import <UIKit/UIKit.h>
#import "SparkViewerViewController.h"
@class GLView;
@class SparkViewerViewController;


@interface SparkViewerAppDelegate : NSObject <UIApplicationDelegate> {
    GLView *myGLView;
        
}

-(void)createGLView;
-(CGRect)getWindowBoundsWithBaseLayout:(NSString*) baseLayout ;
-(void)initialize:(NSString *) projectName ;


@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) SparkViewerViewController *sparkViewerViewController;
 

@end
