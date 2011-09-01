#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import <QuartzCore/QuartzCore.h>
#import <CoreMotion/CMMotionManager.h>

#ifdef __cplusplus
    #include <spark/DemoApp.h>
#endif

#import "EventManager.h"

@interface GLView : UIView {
    
    @private
        
    EAGLContext *glContext;    
    GLuint framebuffer;
    GLuint colorRenderbuffer;
    GLint width;
    GLint height;
    
#ifdef __cplusplus
    spark::DemoApp *myDemoApp;
#endif
    
    BOOL animating;
    EventManager *eventManager;
    CADisplayLink *displayLink;
    CMMotionManager *motionManager;
    

}

- (void) render:(id)sender;
- (void) startAnimation;
- (void) stopAnimation;

@end
