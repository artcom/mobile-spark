// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include <masl/Logger.h>
#import "Camera.h"


@implementation Camera

@synthesize videoPreviewLayer;


+ (Camera *)instance {
    static Camera *instance = NULL;
    
    @synchronized(self)
    {
        if (!instance)
            instance = [[self alloc] init];
        
        return instance;
    }
}


- (id)init {
    if (!(self = [super init]))
		return nil;
	
    _bgraTexture = NULL;
    _pixelBuffer = NULL;
    
	// Grab the back-facing camera
	AVCaptureDevice *backFacingCamera = nil;
	NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	for (AVCaptureDevice *device in devices) 
	{
		if ([device position] == AVCaptureDevicePositionBack) 
		{
			backFacingCamera = device;
		}
	}

	// Create the capture session
	captureSession = [[AVCaptureSession alloc] init];
	
    // fill configuration queue
    [captureSession beginConfiguration];
    
	// Add the video input	
	NSError *error = nil;
	videoInput = [[[AVCaptureDeviceInput alloc] initWithDevice:backFacingCamera error:&error] autorelease];
	if ([captureSession canAddInput:videoInput]) 
	{
		[captureSession addInput:videoInput];
	}
	
    // TODO: videoPreviewLayer needed here?
	[self videoPreviewLayer];
    
	// Add the video frame output	
	videoOutput = [[AVCaptureVideoDataOutput alloc] init];
	[videoOutput setAlwaysDiscardsLateVideoFrames:YES];
    
	// Use RGB frames instead of YUV to ease color processing
    // TODO: do YUV colour conversion in shader
	[videoOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
    
    [videoOutput setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
    
	if ([captureSession canAddOutput:videoOutput]) {
		[captureSession addOutput:videoOutput];
	} else {
		AC_ERROR << "Couldn't add video output";
	}
    
    // too much workload e.g. on Iphone 4s (larger cam-resolution)
    [captureSession setSessionPreset:AVCaptureSessionPresetHigh];
    
	//[captureSession setSessionPreset:AVCaptureSessionPreset640x480];
    
    // apply all settings as batch
    [captureSession commitConfiguration];
	
    return self;
}

- (bool)isCameraCapturing {
    return [captureSession isRunning];
}

- (void)startCameraCapture 
{
    AC_DEBUG << "startCameraCapture";
    if (![captureSession isRunning])
	{
		[captureSession startRunning];
        //glGenTextures(1, &textureID);
        
        //-- Create CVOpenGLESTextureCacheRef for optimal CVImageBufferRef to GLES texture conversion.
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
                                                    NULL,
                                                    (__bridge void *)[EAGLContext currentContext],
                                                    NULL, &_videoTextureCache);
        if (err) 
        {
            AC_ERROR<<"Error at CVOpenGLESTextureCacheCreate: "<<err;
        }

	};        
}

- (void)stopCameraCapture
{
    AC_DEBUG<<"stopCameraCapture";
    
   [self cleanUpTextures];
    
    _pixelBuffer = NULL;
    
    CFRelease(_videoTextureCache);
    
    [captureSession stopRunning];
}

- (void) cleanUpTextures
{
    if (_bgraTexture)
    {
        CFRelease(_bgraTexture);
        _bgraTexture = NULL;
    }
    
    // Periodic texture cache flush every frame
    CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
    
    if (!_videoTextureCache)
    {
        AC_ERROR<<"No video texture cache";
    }
    
    _textureID = 0;
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    [self cleanUpTextures];
    
    _pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);

	_height = CVPixelBufferGetHeight(_pixelBuffer);
	_width = CVPixelBufferGetWidth(_pixelBuffer);
    
    CVReturn err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, 
                                                                _videoTextureCache,
                                                                _pixelBuffer,
                                                                NULL,
                                                                GL_TEXTURE_2D,
                                                                GL_RGBA,
                                                                _width,
                                                                _height,
                                                                GL_BGRA,
                                                                GL_UNSIGNED_BYTE,
                                                                0,
                                                                &_bgraTexture);
    if(err)
    {
        AC_ERROR<<"could not update camera texture";
    }
    
    _textureID = CVOpenGLESTextureGetName(_bgraTexture);
    
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
	//AC_TRACE<<"captureOutput width: "<<_width <<" height: "<<_height;
}

- (int)getWidth
{
    return _width;
}
- (int)getHeight
{
    return _height;
}
- (GLuint)getTextureID
{
    return _textureID;
}



- (AVCaptureVideoPreviewLayer *)videoPreviewLayer;
{
	if (videoPreviewLayer == nil)
	{
		videoPreviewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:captureSession];
        [videoPreviewLayer setVideoGravity:AVLayerVideoGravityResizeAspectFill];
	}
	
	return videoPreviewLayer;
}



@end