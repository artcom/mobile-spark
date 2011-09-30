#import "SparkViewerAppDelegate.h"
#import "GLView.h"
#import "SparkViewerViewController.h"

@implementation SparkViewerAppDelegate

@synthesize isPortrait;
@synthesize window;
@synthesize myGLView;
@synthesize sparkViewerViewController=_sparkViewerViewController;


- (void)createGLView
{
    myGLView = [[GLView alloc]initWithFrame:[window bounds]];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    SparkViewerViewController *aViewController = [[SparkViewerViewController alloc] initWithNibName:nil bundle:nil];
    [self setSparkViewerViewController:aViewController];
    [aViewController release];
    // Override point for customization after application launch
    [window makeKeyAndVisible];
    
    // create a pointer to a dictionary
    NSDictionary *dictionary;
    // read environment from application bundle
    NSBundle* bundle = [NSBundle mainBundle];
    NSString * finalPath = [bundle pathForResource:@"Environment" ofType:@"plist"];
    dictionary = [NSDictionary dictionaryWithContentsOfFile:finalPath];
    for (id key in dictionary) {
        NSLog(@"bundle: key=%@, value=%@", key, [dictionary objectForKey:key]);
        //putenv([NSString stringWithFormat:@"%@=%@", key, [dictionary objectForKey:key]]);
    }
    
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
