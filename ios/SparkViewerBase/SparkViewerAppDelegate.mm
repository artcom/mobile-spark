// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#import "SparkViewerAppDelegate.h"
#import "GLView.h"
#import "SparkViewerViewController.h"
#include <masl/Logger.h>

#include <spark/BaseApp.h>


@implementation SparkViewerAppDelegate

@synthesize window;

@synthesize sparkViewerViewController;


- (void)createGLView
{
    myGLView = [[GLView alloc]initWithFrame:[window bounds]];
}


-(CGRect)getWindowBoundsWithBaseLayout:(NSString*) baseLayout {
    
    bool dummyForPortrait = false; 
    
    CGRect windowBounds = [[UIScreen mainScreen] bounds];
    std::string filename = spark::findBestMatchedLayout("/main",windowBounds.size.width*[[UIScreen mainScreen] scale], windowBounds.size.height*[[UIScreen mainScreen] scale], dummyForPortrait);
    
    //[self.sparkViewerViewController setPortrait:isPortrait];
    
    CGRect translate = [window bounds];
    
    if (!dummyForPortrait) {
        float oldwidth = windowBounds.size.width ;
        windowBounds.size.width = windowBounds.size.height;
        windowBounds.size.height = oldwidth;
        
        translate.origin.y=20;  //XXX:  don't ask TODO: ASK!!!
    } else {
        translate.origin.y=40;  //XXX:  don't ask TODO: ASK!!!
        
    }
    
    window.bounds=translate;
     
    return windowBounds;
}

-(void) initialize:(NSString*) projectName 
{
    NSString *path = [[NSBundle mainBundle] resourcePath];
    spark::assetProviderSetup([path UTF8String],[projectName UTF8String]);
    masl::Logger::get().setLoggerTopLevelTag([projectName UTF8String]);
    
    // create download folder
    NSFileManager *filemgr;
    NSArray *dirPaths;
    NSString *docsDir;
    
    dirPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    docsDir = [dirPaths objectAtIndex:0];
    filemgr =[NSFileManager defaultManager];
    if ([filemgr createDirectoryAtPath:[docsDir stringByAppendingPathComponent:@"downloads"]
           withIntermediateDirectories:YES attributes:nil error: NULL] == NO) {
        NSLog(@"____ download folder creation failed");    
    }
    [filemgr release];
    
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    SparkViewerViewController *aViewController = [[SparkViewerViewController alloc] initWithNibName:nil bundle:nil];
    [self setSparkViewerViewController:aViewController];
    [aViewController release];
    
    // create a pointer to a dictionary
    NSDictionary *dictionary;
    // read environment from application bundle
    NSBundle* bundle = [NSBundle mainBundle];
    NSString * finalPath = [bundle pathForResource:@"Environment" ofType:@"plist"];
    dictionary = [NSDictionary dictionaryWithContentsOfFile:finalPath];
    for (id key in dictionary) {
        NSString* envstring = [NSString stringWithFormat:@"%@=%@", key, [dictionary objectForKey:key]];
        const char *env = [envstring UTF8String]; 
        putenv((char*)env);
    }
    masl::Logger::get().setSeverity();
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]] autorelease];
    
    
    
    
    [self createGLView];
    self.window.rootViewController = self.sparkViewerViewController;
    
    //[self.sparkViewerViewController setPortrait:self.isPortrait];

    [self.window addSubview:self.sparkViewerViewController.view];
    [self.sparkViewerViewController.view addSubview:myGLView];
    [self.window makeKeyAndVisible];
    
    [myGLView startAnimation];
    
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    
    [myGLView stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    
    [myGLView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
}

- (void)dealloc
{
    [window release];
    
    [myGLView release];
    
    [self.sparkViewerViewController release];
    
    [super dealloc];
}

@end
