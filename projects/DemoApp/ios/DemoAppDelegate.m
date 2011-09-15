#import "DemoAppDelegate.h"
#import "DemoGLView.h"

@implementation DemoAppDelegate

- (void)createGLView
{
    myGLView = [[DemoGLView alloc]initWithFrame:[window bounds]];
}

- (void)dealloc
{
    [super dealloc];
}

@end
