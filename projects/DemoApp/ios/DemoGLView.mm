#import "DemoGLView.h"
#include <DemoApp.h>


@implementation DemoGLView

- (void) createApp
{
    myApp = new demoapp::DemoApp();
    MSAAQuality = useNoneMSAA;
}

- (void)dealloc
{
    [super dealloc];
}

@end
