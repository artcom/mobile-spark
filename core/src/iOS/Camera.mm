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
	
	// Add the video input	
	NSError *error = nil;
	videoInput = [[[AVCaptureDeviceInput alloc] initWithDevice:backFacingCamera error:&error] autorelease];
	if ([captureSession canAddInput:videoInput]) 
	{
		[captureSession addInput:videoInput];
	}
	
	[self videoPreviewLayer];
	// Add the video frame output	
	videoOutput = [[AVCaptureVideoDataOutput alloc] init];
	[videoOutput setAlwaysDiscardsLateVideoFrames:YES];
	// Use RGB frames instead of YUV to ease color processing
	[videoOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
    
    [videoOutput setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
    
	if ([captureSession canAddOutput:videoOutput]) {
		[captureSession addOutput:videoOutput];
	} else {
		AC_ERROR << "Couldn't add video output";
	}
    [captureSession setSessionPreset:AVCaptureSessionPresetHigh];
	//[captureSession setSessionPreset:AVCaptureSessionPreset640x480];
	
    return self;
}

- (bool)isCameraCapturing {
    return [captureSession isRunning];
}

- (void)startCameraCapture {
    AC_DEBUG << "startCameraCapture";
    if (![captureSession isRunning])
	{
		[captureSession startRunning];
        glGenTextures(1, &textureID);

	};        
}

- (void)updateCameraTexture {
    // maybe move gl binding from capture output here
}


- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
	height = CVPixelBufferGetHeight(pixelBuffer);
	width = CVPixelBufferGetWidth(pixelBuffer);
	AC_TRACE<<"captureOutput width: "<<width <<" height: "<<height;
    
    
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// This is necessary for non-power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// Using BGRA extension to pull in video frame data directly
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, CVPixelBufferGetBaseAddress(pixelBuffer));
    
	CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);

}


- (void)stopCameraCapture
{
    AC_DEBUG<<"stopCameraCapture";
    
    glDeleteTextures(1, &textureID);
    textureID = 0;
    [captureSession stopRunning];
    
}



- (int)getWidth
{
    return width;
}
- (int)getHeight
{
    return height;
}
- (GLuint)getTextureID
{
    return textureID;
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