#import <UIKit/UIKit.h>
#import "SparkViewerViewController.h"
@class GLView;
@class SparkViewerViewController;


@interface SparkViewerAppDelegate : NSObject <UIApplicationDelegate> {
    GLView *myGLView;
    UIWindow *window;
    SparkViewerViewController *sparkViewerViewController;
    
    @protected
        bool isPortrait;

    
}

-(void)createGLView;
-(CGRect)getWindowBoundsWithBaseLayout:(NSString*) baseLayout ;
-(void)initialize:(NSString *) projectName ;


@property bool isPortrait;
@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) GLView *myGLView;
@property (nonatomic, retain) SparkViewerViewController *sparkViewerViewController;
 

@end
