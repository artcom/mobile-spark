#import "TemplateAppDelegate.h"
#import "TemplateGLView.h"

@implementation TemplateAppDelegate

- (void)createGLView
{
    myGLView = [[TemplateGLView alloc]initWithFrame:[window bounds]];
}

- (void)dealloc
{
    [super dealloc];
}

@end
