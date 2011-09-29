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
    NSLog(@"isPortrait %i", isPortrait);
    NSLog(@"isPortrait %d", interfaceOrientation);

     NSLog(@"interfaceOrientation == UIInterfaceOrientationPortrait %i", interfaceOrientation == UIInterfaceOrientationPortrait);
    NSLog(@"interfaceOrientation == UIInterfaceOrientationLandscapeLeft %i", interfaceOrientation == UIInterfaceOrientationLandscapeLeft);
    NSLog(@"interfaceOrientation == UIInterfaceOrientationLandscapeRight %i", interfaceOrientation == UIInterfaceOrientationLandscapeRight);

    //if(isPortrait) return (interfaceOrientation == UIInterfaceOrientationPortrait);
    return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft);
}

-(void)setPortrait:(bool)thePortrait {
    isPortrait = thePortrait;
    
}

-(void)dealloc {
    [super dealloc];
}

@end