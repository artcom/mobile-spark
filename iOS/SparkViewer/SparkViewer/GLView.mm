#import "GLView.h"
#include <spark/DemoApp.h>
#import "EventManager.h"

@implementation GLView

+ (Class) layerClass
{    
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        // Apple changed EGL a lot, it is not possible to render to the display directly. 
        // We have to render into a framebuffer, which is displayed to the user
        NSLog(@"in initWithFrame");

        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
        
        glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (glContext == nil)
        {
            NSLog(@"OpenGL ES2 not supported trying OpenGL ES1");
            
            glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            if (glContext == nil) {
                NSLog(@"OpengGL ES1 is not supported, aborting rendering");
                [self release];
                return nil;
            }
        }
        
        [EAGLContext setCurrentContext:glContext];
        
        //create framebuffer
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        
        //create color renderbuffer
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [glContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
        
        //DemoApp
        NSLog(@"width: %d,  height: %d ", width, height);
        NSString *path = [[NSBundle mainBundle] resourcePath];
        myDemoApp = new spark::DemoApp();
        myDemoApp->setup((displayLink.timestamp*1000.0),[path UTF8String], "assets/layouts/main.spark");
        myDemoApp->onSizeChanged(width, height);
        
        //Motion Events
        UITapGestureRecognizer *singleFingerTap = 
        [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleSingleTap:)];
        [self addGestureRecognizer:singleFingerTap];
       
        motionManager = [[CMMotionManager alloc] init];
        motionManager.deviceMotionUpdateInterval = 1.0/60.0; //60Hz
        
        [[EventManager alloc] initWithSourceView:self targetApp:myDemoApp];
        
        
    }
    return self;
}


- (void)render:(id)sender 
{
    CMAttitude *attitude = motionManager.deviceMotion.attitude;
    //NSLog(@"Euler Angles roll: %f pitch: %f yaw: %f", attitude.roll, attitude.pitch, attitude.yaw);
    
    myDemoApp->onFrame((displayLink.timestamp*1000.0));    


    
    [glContext presentRenderbuffer:GL_RENDERBUFFER];    
}

- (void)startAnimation
{
    if (!animating)
    {
        //init Animation loop. fires at 30 hz
        displayLink = [self.window.screen displayLinkWithTarget:self selector:@selector(render:)];
        [displayLink setFrameInterval:2];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        animating = TRUE;
    }
    
    //works only on phones with gyroscope(iPhone4, iPad2)
    if (motionManager.deviceMotionAvailable)
    {
        [motionManager startDeviceMotionUpdates];
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        [displayLink invalidate];
        displayLink = nil;
        animating = FALSE;
    }
    
    if (motionManager.deviceMotionActive)
    {
        [motionManager stopDeviceMotionUpdates];
    }
}

- (void)dealloc
{
    
    if (framebuffer)   
    {
        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;
    }
    
    if (colorRenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
    
    [glContext release];
    glContext = nil;
    
    [motionManager release];
    motionManager = nil;
    
    [super dealloc];
}

@end
