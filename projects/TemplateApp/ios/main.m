#import <UIKit/UIKit.h>

int main(int argc, char *argv[])
{
    NSLog(@"in main");
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"TemplateAppDelegate");
    [pool release];
    return retVal;
}