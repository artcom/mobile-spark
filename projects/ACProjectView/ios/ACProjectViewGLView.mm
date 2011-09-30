#import "ACProjectViewGLView.h"

#include <spark/AppProvider.h>
#include <ACProjectView.h>


@implementation ACProjectViewGLView

- (void) createApp
{
    spark::AppProvider::get().setApp(spark::BaseAppPtr(new acprojectview::ACProjectView()));
    //activate Multisample anti aliasing with useMSAA2x, useMSAA4x, useMSAA8x
    MSAAQuality = useNoneMSAA;
    
    //choose framerate. Default is 1 -> 60hz. 2 -> 30 hz. Evertything < 1 is undefined.  
    //frameInterval = 1;
}

- (void)dealloc
{
    [super dealloc];
}

@end
