#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import <QuartzCore/QuartzCore.h>
#import <CoreMotion/CMMotionManager.h>

#ifdef __cplusplus
    #include <spark/BaseApp.h>
#endif

#import "EventManager.h"
#import "Sensors.h"


typedef enum {
    useNoneMSAA = 0,
    useMSAA2x = 2,
    useMSAA4x = 4,
    useMSAA8x = 8,
} GLViewMSAAQuality;

@interface GLView : UIView {

    @private
        EAGLContext *glContext;    
        GLuint framebuffer;
        GLuint renderbuffer;
        GLuint depthRenderbuffer;
        GLuint multisamplingFramebuffer;
        GLuint multisamplingRenderbuffer;
        GLint width;
        GLint height;
        BOOL animating;
        EventManager *eventManager;
        Sensors *sensorManager;

        CADisplayLink *displayLink;
           
    @protected

    #ifdef __cplusplus
        spark::BaseAppPtr _myApp;
        GLViewMSAAQuality MSAAQuality;
        int frameInterval;
    #endif

}


- (void) createApp;
- (void) render:(id)sender;
- (void) renderWithMSAA:(id)sender;
- (void) startAnimation;
- (void) stopAnimation;


@end
