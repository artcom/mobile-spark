#import <UIKit/UIKit.h>
#import "SparkViewerViewController.h"
@class GLView;
@class SparkViewerViewController;


@interface SparkViewerAppDelegate : NSObject <UIApplicationDelegate> {
    GLView *myGLView;
    UIWindow *window;
    //SparkViewerViewController *sparkViewerViewController;

}

-(void)createGLView;

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) GLView *myGLView;
@property (nonatomic, retain) SparkViewerViewController *sparkViewerViewController;
 

@end
