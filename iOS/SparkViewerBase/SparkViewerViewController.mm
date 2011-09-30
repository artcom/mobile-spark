#include "SparkViewerViewController.h"
#import "GLView.h"


@implementation SparkViewerViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nil bundle:nil];
    self.view.contentMode =UIViewContentModeLeft;
    return self;
}



-(void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{   
    if(isPortrait) return NO;
    return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft);
}

-(void)setPortrait:(bool)thePortrait {
    isPortrait = thePortrait;
    
}

-(void)dealloc {
    [super dealloc];
}

@end