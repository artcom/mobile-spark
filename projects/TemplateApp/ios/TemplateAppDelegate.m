#import "TemplateAppDelegate.h"
#import "TemplateAppGLView.h"

@implementation TemplateAppDelegate

- (void)createGLView
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];    
    [self initialize:@"TemplateApp"];
    myGLView = [[TemplateAppGLView alloc]initWithFrame: [self getWindowBoundsWithBaseLayout:@"/main"]];}

- (void)dealloc
{
    [super dealloc];
}

@end
