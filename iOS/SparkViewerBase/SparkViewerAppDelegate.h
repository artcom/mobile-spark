#import <UIKit/UIKit.h>
@class GLView;

@interface SparkViewerAppDelegate : NSObject <UIApplicationDelegate> {
    GLView *myGLView;
    UIWindow *window;
}

-(void)createGLView;

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) GLView *myGLView;

@end