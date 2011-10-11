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