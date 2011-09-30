#import "SparkViewerAppDelegate.h"
#import "GLView.h"
#import "SparkViewerViewController.h"
#include <spark/BaseApp.h>


@implementation SparkViewerAppDelegate

@synthesize isPortrait;
@synthesize window;
@synthesize myGLView;
@synthesize sparkViewerViewController=_sparkViewerViewController;


- (void)createGLView
{
    myGLView = [[GLView alloc]initWithFrame:[window bounds]];
}


-(CGRect)getWindowBoundsWithBaseLayout:(NSString*) baseLayout {
    isPortrait = NO; 
    CGRect windowBounds = [[UIScreen mainScreen] bounds];
    std::string filename = spark::findBestMatchedLayout("/main",windowBounds.size.width, windowBounds.size.height, isPortrait);
    CGRect translate = [window bounds]; 
    if (!isPortrait) {
        float oldwidth = windowBounds.size.width ;
        windowBounds.size.width = windowBounds.size.height;
        windowBounds.size.height = oldwidth;
        translate.origin.y=20;  //XXX:  don't ask
    } else {
        translate.origin.y=40;  //XXX:  don't ask
    }
    window.bounds=translate;
    return windowBounds;
}

-(void) initializeAssetProvider:(NSString*) projectName 
{
    NSString *path = [[NSBundle mainBundle] resourcePath];
    spark::assetProviderSetup([path UTF8String],[projectName UTF8String]);
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    SparkViewerViewController *aViewController = [[SparkViewerViewController alloc] initWithNibName:nil bundle:nil];
    [self setSparkViewerViewController:aViewController];
    [aViewController release];

    
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]] autorelease];
    
    
    
    
    [self createGLView];
    self.window.rootViewController = self.sparkViewerViewController;
    [self.sparkViewerViewController setPortrait:self.isPortrait];

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
    
    [_sparkViewerViewController release];

    
    [super dealloc];
}

@end
