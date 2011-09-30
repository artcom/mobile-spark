#import "DemoAppDelegate.h"
#import "DemoGLView.h"

@implementation DemoAppDelegate

- (void)createGLView
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];    
    [self initializeAssetProvider:@"DemoApp"];
    myGLView = [[DemoGLView alloc]initWithFrame: [self getWindowBoundsWithBaseLayout:@"/main"]];}

- (void)dealloc
{
    [super dealloc];
}

@end
