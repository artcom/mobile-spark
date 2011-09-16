#import "TemplateAppGLView.h"
#include <TemplateApp.h>


@implementation TemplateAppGLView

- (void) createApp
{
    myApp = new templateapp::TemplateApp();
}

- (void)dealloc
{
    [super dealloc];
}

@end
