#import "ACProjectViewDelegate.h"
#import "ACProjectViewGLView.h"
#include <spark/BaseApp.h>

@implementation ACProjectViewDelegate

- (void)createGLView
{
[[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];    
    
    
    isPortrait = NO; 
    NSString *path = [[NSBundle mainBundle] resourcePath];
    spark::assetProviderSetup([path UTF8String],"ACProjectView");
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
    myGLView = [[ACProjectViewGLView alloc]initWithFrame:windowBounds];

}

- (void)dealloc
{
    [super dealloc];
}

@end
