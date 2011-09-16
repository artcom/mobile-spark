#import "ACProjectViewGLView.h"
#include <ACProjectView.h>


@implementation ACProjectViewGLView

- (void) createApp
{
    myApp = new acprojectview::ACProjectView();
}

- (void)dealloc
{
    [super dealloc];
}

@end
