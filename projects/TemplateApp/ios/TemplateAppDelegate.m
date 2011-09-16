#import "TemplateAppDelegate.h"
#import "TemplateAppGLView.h"

@implementation TemplateAppDelegate

- (void)createGLView
{
    myGLView = [[TemplateAppGLView alloc]initWithFrame:[window bounds]];
}

- (void)dealloc
{
    [super dealloc];
}

@end
