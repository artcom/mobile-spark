#import "ACProjectViewDelegate.h"
#import "ACProjectViewGLView.h"

@implementation ACProjectViewDelegate

- (void)createGLView
{
    myGLView = [[ACProjectViewGLView alloc]initWithFrame:[window bounds]];
}

- (void)dealloc
{
    [super dealloc];
}

@end
