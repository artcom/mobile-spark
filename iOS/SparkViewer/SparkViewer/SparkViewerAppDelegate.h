//
//  SparkViewerAppDelegate.h
//  SparkViewer
//
//  Created by Arvid Hansen Diaz on 15.08.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
@class GLView;

@interface SparkViewerAppDelegate : NSObject <UIApplicationDelegate> {
    GLView *myGLView;
    UIWindow *window;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) GLView *myGLView;

@end
