//
//  GLView.h
//  mobile-spark
//
//  Created by Arvid Hansen Diaz on 15.08.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import <QuartzCore/QuartzCore.h>

//@class DemoApp;

@interface GLView : UIView {
    
    @private
        
    EAGLContext *glContext;
    
    GLuint framebuffer;
    GLuint colorRenderbuffer;
    CADisplayLink *displayLink;
    
    GLint width;
    GLint height;
    
    BOOL animating;
    
    //spark::DemoApp *myDemoApp;
    
}

- (void) render:(id)sender;
- (void) startAnimation;
- (void) stopAnimation;

@end
