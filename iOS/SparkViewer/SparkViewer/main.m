//
//  main.m
//  SparkViewer
//
//  Created by Arvid Hansen Diaz on 15.08.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[])
{
    NSLog(@"in main");
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"SparkViewerAppDelegate");
    [pool release];
    return retVal;
}