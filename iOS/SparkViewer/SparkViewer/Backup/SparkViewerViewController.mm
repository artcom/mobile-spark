//
//  SparkViewerViewController.m
//  SparkViewer
//
//  Created by Arvid Hansen Diaz on 04.08.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "SparkViewerViewController.h"

#include "CppEntry.h"

@implementation SparkViewerViewController

- (void)dealloc
{
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
 
    NSString *stringPath = [[NSBundle mainBundle] pathForResource:@"test_image" ofType:@"png"];
    UIImage *imageObj = [[UIImage alloc] initWithContentsOfFile:stringPath];
    
    UIImageView *imageView = [[UIImageView alloc] initWithImage:imageObj];
    [imageView setFrame:CGRectMake(300, 400, 200, 200)];
    
    [self.view addSubview:imageView];
 
    [imageView release];
    [imageObj release];
}


- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return YES;
}

- (IBAction)buttonPressed:(id)sender {
    NSLog(@"Button pressed!");
    
    
    cppcore::CppEntry *myEntry = new cppcore::CppEntry();
    NSString *sparkFilePath = [[NSBundle mainBundle] pathForResource:@"test" ofType:@"spark"];
    const char *convertedSparkFilePath = [sparkFilePath UTF8String];
    
    myEntry->loadSpark(convertedSparkFilePath);

    delete myEntry;
}
@end
