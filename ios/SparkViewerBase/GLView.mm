// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#import "GLView.h"
#include <mar/openGL_functions.h>
#include <spark/BaseApp.h>
#include <spark/AppProvider.h>

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
        [self createApp];
        _myApp = spark::AppProvider::get().getApp();
        
        // Initialization code
        // Apple changed EGL a lot, it is not possible to render to the display directly. 
        // We have to render into a framebuffer, which is displayed to the user
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = TRUE;
        //eaglLayer.contentsScale=2;
        
        self.contentScaleFactor = [[UIScreen mainScreen] scale];
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

        if (![self setupFramebuffer]) return nil;
        //[self resizeView:self.bounds]
        
        //setup DemoApp
        NSString *path = [[NSBundle mainBundle] resourcePath];
        _myApp->setup((0.0),[path UTF8String], width, height);
        
        eventManager = [[EventManager alloc] initWithSourceView:self];
        sensorManager = [[Sensors alloc] init];

    }
    return self;
}

- (BOOL) setupFramebuffer
{
    [EAGLContext setCurrentContext:glContext];
    
    //create framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    //create color renderbuffer
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    
    [glContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
    if (MSAAQuality > 0) 
    {
        //Create Multisampling Buffer
        glGenFramebuffers(1, &multisamplingFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, multisamplingFramebuffer);
        
        glGenRenderbuffers(1, &multisamplingRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, multisamplingRenderbuffer);
        
        //Use 4x Multisample anti-aliasing
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, MSAAQuality, GL_RGBA8_OES, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, multisamplingRenderbuffer);
        
        //create depthbuffer with MSAA
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, MSAAQuality, GL_DEPTH_COMPONENT16, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    } 
    else 
    {
        //create depthbuffer
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    }
    //Test Frame Buffer
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
    if(status != GL_FRAMEBUFFER_COMPLETE) 
    {
        
        NSLog(@"failed to make complete framebuffer object %x", status);
        return NO;
        
    }
    
    return YES;
}

- (void) tearDownFramebuffer
{
    [EAGLContext setCurrentContext:glContext];
    
    //delete framebuffer
    if(framebuffer)
    {
        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;
    }
    
    if(renderbuffer)
    {
        //delete color renderbuffer
        glDeleteRenderbuffers(1, &renderbuffer);
        renderbuffer = 0;
    }
    
    //delete depthbuffer
    if(depthRenderbuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    
    if (MSAAQuality > 0) 
    {
        //delete Multisampling Buffer
        if(multisamplingFramebuffer)
        {
            glDeleteFramebuffers(1, &multisamplingFramebuffer);
            multisamplingFramebuffer = 0;
        }
        if(multisamplingRenderbuffer)
        {
            glDeleteRenderbuffers(1, &multisamplingRenderbuffer);
            multisamplingRenderbuffer = 0;
        }
    } 
}

- (void) resizeView: (CGRect) newBounds
{
    self.bounds = newBounds;
    
    // following code will re-init our RenderBuffer with
    // the new dimensions
    [self tearDownFramebuffer];
    [self setupFramebuffer];
    
    // now pass event to application
    [eventManager onResize:newBounds];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent*)event {
    [eventManager onTouchDown:[touches anyObject]];
}


- (void)render:(id)sender 
{
    [EAGLContext setCurrentContext:glContext];
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      
    //render
    NSString *frameEvent = [NSString stringWithFormat:@"<StageEvent type='frame' time='%f'/>", displayLink.timestamp * 1000.0];
    _myApp->onEvent([frameEvent UTF8String]);    
    _myApp->handleEvents();
    
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    [glContext presentRenderbuffer:GL_RENDERBUFFER];  
}

- (void)renderWithMSAA:(id)sender
{
    [EAGLContext setCurrentContext:glContext];
    glBindFramebuffer(GL_FRAMEBUFFER, multisamplingFramebuffer);
     
    //render
    NSString *frameEvent = [NSString stringWithFormat:@"<StageEvent type='frame' time='%f'/>", displayLink.timestamp * 1000.0];
    _myApp->onEvent([frameEvent UTF8String]);  
    _myApp->handleEvents();
        
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, framebuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, multisamplingFramebuffer);
    glResolveMultisampleFramebufferAPPLE();
    
    const GLenum discards[]  = {GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT};
    glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE,2,discards);

    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    [glContext presentRenderbuffer:GL_RENDERBUFFER];  
        
}
- (void)onResume
{ 
}
- (void)startAnimation
{
    if (!animating)
    {
        //init Animation loop. fires at 60hz default, set frameInterval to 2 for 30hz
        if (MSAAQuality > 0) {
            displayLink = [self.window.screen displayLinkWithTarget:self selector:@selector(renderWithMSAA:)];
        }
        else {
            displayLink = [self.window.screen displayLinkWithTarget:self selector:@selector(render:)];
        }
        if (frameInterval) {
            [displayLink setFrameInterval:frameInterval];
        }
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        animating = TRUE;
        
        [sensorManager enable];
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
    
    [sensorManager disable];
}


- (void)dealloc
{
    // delete all framebuffer related stuff
    [self tearDownFramebuffer];
    
    [glContext release];
    glContext = nil;
    
    [eventManager release];
    eventManager = nil;
    
    [sensorManager release];
    sensorManager = nil;
    
    [super dealloc];
}

@end
