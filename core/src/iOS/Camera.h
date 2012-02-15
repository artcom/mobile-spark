// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#import <Foundation/NSObject.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

//#import <UIKit/UIKit.h>


@interface Camera : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate> {
	AVCaptureVideoPreviewLayer *videoPreviewLayer;
	AVCaptureSession *captureSession;
	AVCaptureDeviceInput *videoInput;
	AVCaptureVideoDataOutput *videoOutput;
    CVImageBufferRef pixelBuffer;
    GLuint textureID;
    int width;
    int height;

    CVOpenGLESTextureRef _bgraTexture;
    CVOpenGLESTextureCacheRef _videoTextureCache;

}
+ (Camera *) instance;
- (void)startCameraCapture;

@property(readonly) AVCaptureVideoPreviewLayer *videoPreviewLayer;


- (int)getWidth;
- (int)getHeight;
- (GLuint)getTextureID;
- (void)startCameraCapture;
- (void)updateCameraTexture;
- (void)stopCameraCapture;
- (bool)isCameraCapturing;


@end