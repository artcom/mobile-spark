//
//  SparkViewerAppDelegate.h
//  SparkViewer
//
//  Created by Arvid Hansen Diaz on 04.08.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SparkViewerViewController;

@interface SparkViewerAppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet SparkViewerViewController *viewController;

@end
