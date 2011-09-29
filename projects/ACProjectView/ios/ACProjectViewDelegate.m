#import "ACProjectViewDelegate.h"
#import "ACProjectViewGLView.h"

@implementation ACProjectViewDelegate

- (void)createGLView
{
    isPortrait = NO; 
    CGRect rect1 = [window bounds];

    if (!isPortrait) {
        float oldwidth = rect1.size.width ;
        rect1.size.width = rect1.size.height+20;
        rect1.size.height = oldwidth-20;
        CGRect translate = [window bounds]; 
        translate.origin.y=20;
        window.bounds=translate;
    }
    

    myGLView = [[ACProjectViewGLView alloc]initWithFrame:rect1];

}

- (void)dealloc
{
    [super dealloc];
}

@end
