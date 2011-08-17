//
//  GLView.m
//  mobile-spark
//
//  Created by Arvid Hansen Diaz on 15.08.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "GLView.h"
#include <spark/DemoApp.h>


@implementation GLView

spark::DemoApp *myDemoApp;

// You must implement this method

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
        
        myDemoApp = new spark::DemoApp();
        myDemoApp->setup("", "main.spark");
    }
    return self;
}

- (void)render:(id)sender 
{
    
    //start C++ rendering here
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    glViewport(0, 0, width, height);
    
    glClearColor(0.5f, 0.4f, 0.5f, 1.0f);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    [glContext presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)startAnimation
{
    if (!animating)
    {
        //init Animation loop. fires at 60 hz
        displayLink = [self.window.screen displayLinkWithTarget:self selector:@selector(render:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        animating = TRUE;
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
    
    [super dealloc];
}

@end
