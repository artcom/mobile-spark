#import <UIKit/UIKit.h>

int main(int argc, char *argv[])
{
    NSLog(@"in main");
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"DemoAppDelegate");
    [pool release];
    return retVal;
    
    
}
