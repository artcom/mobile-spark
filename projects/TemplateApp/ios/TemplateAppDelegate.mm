// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#import "TemplateAppDelegate.h"
#import "TemplateAppGLView.h"

#include "spark/AppProvider.h"
#include "TemplateApp.h"

@implementation TemplateAppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    _defaults = [[NSUserDefaults standardUserDefaults] retain];
    
    NSDictionary *appDefaults = [NSDictionary dictionaryWithObject:@"some text ..."
                                                            forKey:@"exampleText"];
    [_defaults registerDefaults:appDefaults];
    
    [_defaults synchronize];
    
    return [super application:application didFinishLaunchingWithOptions:launchOptions];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [super applicationDidBecomeActive:application];
    
    [_defaults synchronize];
    
    // read the contentz of the example textfield
    //NSString *exampleString = [_defaults stringForKey:@"exampleText"];
    
    // the Application Ptr
    templateapp::TemplateAppPtr appPtr = boost::static_pointer_cast
    <templateapp::TemplateApp> (spark::AppProvider::get().getApp());
    
    //appPtr->callSomeMemberFunctions();
    
}

- (void)createGLView
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];    
    [self initialize:@"TemplateApp"];

     // this call will also create the Spark-Application ...
    myGLView = [[TemplateAppGLView alloc]initWithFrame:[[UIScreen mainScreen] bounds]];
}

- (void)dealloc
{
    [super dealloc];
    
    [_defaults release];
}

@end
