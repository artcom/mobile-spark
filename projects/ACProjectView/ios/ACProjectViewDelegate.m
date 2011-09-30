#import "ACProjectViewDelegate.h"
#import "ACProjectViewGLView.h"

@implementation ACProjectViewDelegate

- (void)createGLView {
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];    
    [self initialize:@"ACProjectView"];
    myGLView = [[ACProjectViewGLView alloc]initWithFrame: [self getWindowBoundsWithBaseLayout:@"/main"]];

}

- (void)dealloc
{
    [super dealloc];
}

@end
