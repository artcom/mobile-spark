//
//  GLView.m
//  mobile-spark
//
//  Created by Arvid Hansen Diaz on 15.08.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "GLView.h"
#include <spark/DemoApp.h>
#import "matrix.h"
#import "Shaders.h"
// uniform index

enum {
    
    UNIFORM_MODELVIEW_PROJECTION_MATRIX,
    
    NUM_UNIFORMS
    
};

GLint uniforms[NUM_UNIFORMS];



// attribute index

enum {
    
    ATTRIB_VERTEX,
    
    ATTRIB_COLOR,
    
    NUM_ATTRIBUTES
    
};

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
        [self loadShaders];
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
        
        //NSString *path = [[NSBundle mainBundle] pathForResource:@"assets/layouts" ofType:@"" inDirectory:@"assets/layouts"];
        NSString *path = [[NSBundle mainBundle] resourcePath];
        NSLog(@"%@",path);
        //myDemoApp = new spark::DemoApp();
        //myDemoApp->setup([path UTF8String], "assets/layouts/main.spark");
    }
    return self;
}

- (void)render:(id)sender 
{
    // Replace the implementation of this method to do your own custom drawing
    
    
    
    const GLfloat squareVertices[] = {
        
        -0.5f, -0.5f,
        
        0.5f,  -0.5f,
        
        -0.5f,  0.5f,
        
        0.5f,   0.5f,
        
    };
    
    const GLubyte squareColors[] = {
        
        255, 255,   0, 255,
        
        0,   255, 255, 255,
        
        0,     0,   0,   0,
        
        255,   0, 255, 255,
        
    };
    
    
    
    [EAGLContext setCurrentContext:glContext];
    
    
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    glViewport(0, 0, width, height);
    
    
    
    glClearColor(0.5f, 0.4f, 0.5f, 1.0f);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    
    // use shader program
    
    glUseProgram(program);
    
    
    
    // handle viewing matrices
    
    GLfloat proj[16], modelview[16], modelviewProj[16];
    
    // setup projection matrix (orthographic)
    
    mat4f_LoadOrtho(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f, proj);
    
    // setup modelview matrix (rotate around z)
    
    mat4f_LoadZRotation(rotz, modelview);
    
    // projection matrix * modelview matrix
    
    mat4f_MultiplyMat4f(proj, modelview, modelviewProj);
    
    rotz += 3.0f * M_PI / 180.0f;
    
    
    
    // update uniform values
    
    glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, modelviewProj);
    
    
    
    // update attribute values
    
    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVertices);
    
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    
    glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, 0, squareColors); //enable the normalized flag
    
    glEnableVertexAttribArray(ATTRIB_COLOR);
    
    
    
    // Validate program before drawing. This is a good check, but only really necessary in a debug build.
    
    // DEBUG macro must be defined in your debug configurations if that's not already the case.
    
#if defined(DEBUG)
    
    if (![self validateProgram:program])
        
    {
        
        NSLog(@"Failed to validate program: %d", program);
        
        return;
        
    }
    
#endif
    
    
    
    // draw
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    [glContext presentRenderbuffer:GL_RENDERBUFFER];
    
}

- (BOOL)loadShaders {
    
    
    
    GLuint vertShader, fragShader;
    
    NSString *vertShaderPathname, *fragShaderPathname;
    
    
    
    // create shader program
    
    program = glCreateProgram();
    
    
    
    // create and compile vertex shader
    
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"template" ofType:@"vsh" inDirectory:@"assets/layouts"];
    
    if (!compileShader(&vertShader, GL_VERTEX_SHADER, 1, vertShaderPathname)) {
        
        destroyShaders(vertShader, fragShader, program);
        
        return NO;
        
    }
    
    
    
    // create and compile fragment shader
    
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"template" ofType:@"fsh" inDirectory:@"assets/layouts"];
    
    if (!compileShader(&fragShader, GL_FRAGMENT_SHADER, 1, fragShaderPathname)) {
        
        destroyShaders(vertShader, fragShader, program);
        
        return NO;
        
    }
    
    
    
    // attach vertex shader to program
    
    glAttachShader(program, vertShader);
    
    
    
    // attach fragment shader to program
    
    glAttachShader(program, fragShader);
    
    
    
    // bind attribute locations
    
    // this needs to be done prior to linking
    
    glBindAttribLocation(program, ATTRIB_VERTEX, "position");
    
    glBindAttribLocation(program, ATTRIB_COLOR, "color");
    
    
    
    // link program
    
    if (!linkProgram(program)) {
        
        destroyShaders(vertShader, fragShader, program);
        
        return NO;
        
    }
    
    
    
    // get uniform locations
    
    uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(program, "modelViewProjectionMatrix");
    
    
    
    // release vertex and fragment shaders
    
    if (vertShader) {
        
        glDeleteShader(vertShader);
        
        vertShader = 0;
        
    }
    
    if (fragShader) {
        
        glDeleteShader(fragShader);
        
        fragShader = 0;
        
    }
    
    
    
    return YES;
    
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

- (NSString*) getResourcePath:(NSString*)file:(NSString*)fileType:(NSString*)inDirectory
{
    //NSScanner *scanner = [NSScanner scannerWithString:file];
    //NSString *resourcePath;
    //NSString *resourceType;
    //[scanner scanUpToString:@"." intoString:&resourcePath];
    //[scanner scanString:@"." intoString:NULL];
    //resourceType = [[scanner string] substringToIndex:[scanner scanLocation]];
    return [[NSBundle mainBundle] pathForResource:file ofType:fileType inDirectory:inDirectory]; 
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
