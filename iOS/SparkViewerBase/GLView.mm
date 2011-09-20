#import "GLView.h"
#include <mar/openGL_functions.h>

@implementation GLView

+ (Class) layerClass
{    
    return [CAEAGLLayer class];
}

- (void) createApp
{
    //has to be implemened in child class
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
        [glContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
        
        //create depthbuffer
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        
        //Test Frame Buffer
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
        if(status != GL_FRAMEBUFFER_COMPLETE) {
            
            NSLog(@"failed to make complete framebuffer object %x", status);
            
        }
        
        //DemoApp
        NSString *path = [[NSBundle mainBundle] resourcePath];
        [self createApp];
        myApp->setup((0.0),[path UTF8String], width, height);
        
        NSString *resizeEvent = [NSString stringWithFormat:@"<WindowEvent type='on_resize' newsize='[%d,%d]' oldsize='[%d,%d]'/>", width, height, width, height];
        myApp->onEvent([resizeEvent UTF8String]); 

        //Motion Events
        UITapGestureRecognizer *singleFingerTap = 
        [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleSingleTap:)];
        [self addGestureRecognizer:singleFingerTap];
       
        motionManager = [[CMMotionManager alloc] init];
        motionManager.deviceMotionUpdateInterval = 1.0/60.0; //60Hz
        
        eventManager = [[EventManager alloc] initWithSourceView:self targetApp:myApp];
    }
    return self;
}


- (void)render:(id)sender 
{
    [EAGLContext setCurrentContext:glContext];
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    CMAttitude *attitude = motionManager.deviceMotion.attitude;
    //NSLog(@"Euler Angles roll: %f pitch: %f yaw: %f", attitude.roll, attitude.pitch, attitude.yaw);
    
    //render
    NSString *frameEvent = [NSString stringWithFormat:@"<StageEvent type='frame' time='%f'/>", displayLink.timestamp * 1000.0];
    myApp->onEvent([frameEvent UTF8String]);    
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [glContext presentRenderbuffer:GL_RENDERBUFFER];  
        
}
- (void)onResume
{ 
}
- (void)startAnimation
{
    if (!animating)
    {
        //init Animation loop. fires at 60 hz
        displayLink = [self.window.screen displayLinkWithTarget:self selector:@selector(render:)];
        [displayLink setFrameInterval:1];
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
    
    if (depthRenderbuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    
    [glContext release];
    glContext = nil;
    
    [eventManager release];
    eventManager = nil;
    
    [motionManager release];
    motionManager = nil;
    
    [super dealloc];
}

@end
