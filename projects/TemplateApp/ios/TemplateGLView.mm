#import "TemplateGLView.h"
#include <TemplateApp.h>


@implementation TemplateGLView

- (void) createApp
{
    myApp = new templateapp::TemplateApp();
}

- (void)dealloc
{
    [super dealloc];
}

@end
