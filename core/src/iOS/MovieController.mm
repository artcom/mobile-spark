// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "MovieController.h"

#import <AVFoundation/AVFoundation.h>
#import <CoreFoundation/CoreFoundation.h>

namespace ios {
    
    struct AVStruct
    {
        AVAssetReaderTrackOutput *trackOut;
        AVAssetReader *assetReader;
        
        AVStruct():trackOut(NULL),assetReader(NULL){};
        ~AVStruct()
        {
            if(trackOut) [trackOut release];
            if(assetReader) [assetReader release];
        };
    };
    
    MovieController::MovieController():_bgraTexture(NULL),
    _avStruct(AVStructPtr(new AVStruct))
    {
        //-- Create CVOpenGLESTextureCacheRef for optimal CVImageBufferRef to GLES texture conversion.
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
                                                    NULL,
                                                    (__bridge void *)[EAGLContext currentContext],
                                                    NULL, &_videoTextureCache);
        if (err) 
        {
            NSLog(@"Error at CVOpenGLESTextureCacheCreate %d", err);
            return;
        }
    }
    
    MovieController::~MovieController()
    {
        //glDeleteTextures(1, &textureID);
        
        CFRelease(_videoTextureCache);
    }
    
    void MovieController::playMovie(const std::string &filePath)
    {
        NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:filePath.c_str()]];
        
        AVURLAsset *asset = [AVURLAsset URLAssetWithURL:url options:nil];
        
        NSString *tracksKey = @"tracks";
        
        [asset loadValuesAsynchronouslyForKeys:[NSArray arrayWithObject:tracksKey] 
                             completionHandler:
         ^{
             // completion code
             
             printf("Asset loading complete for: %s\n",[[asset.URL absoluteString] UTF8String]);
             
             //NSArray *tracksArray = [asset tracksWithMediaCharacteristic:AVMediaCharacteristicVisual];
             NSArray *tracksArray = [asset tracksWithMediaType:AVMediaTypeVideo];
             
             printf("ALL TRACK # : %d\n",[[asset tracks] count]);
             printf("VIDEO TRACK # : %d\n",[tracksArray count]);
             
             
             if([tracksArray count])
             {
                 NSError *error = nil;
                 
                 AVAssetTrack *videoTrack = [tracksArray objectAtIndex:0];
                 
                 NSMutableDictionary* outSettings = [NSMutableDictionary 
                                                     dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] 
                                                     forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
                 
//                 m_trackOutput = 
                 _avStruct->trackOut = [[AVAssetReaderTrackOutput alloc] initWithTrack:videoTrack outputSettings:outSettings];
                 
                 _avStruct->assetReader = [[AVAssetReader alloc] initWithAsset:asset error:&error];
                 
                 if(!error)
                 {
                     [_avStruct->assetReader addOutput:_avStruct->trackOut];
                     if (![_avStruct->assetReader startReading]) 
                     {
                         NSLog(@"Error: AssetReader could not start reading ...");
                     }
                 }
                 else
                 {
                     NSLog(@"Error: AssetReader could not be initialized ...");
                 }
             }
             
             
         }];
    }
    
    void MovieController::stop()
    {
    
    }
    
    void MovieController::pause()
    {
    
    }
    
    void MovieController::reset()
    {
    
    }
    
    void MovieController::copyNextFrameToTexture()
    {
        CMSampleBufferRef sampleBuffer = NULL;

        if([_avStruct->assetReader status] == AVAssetReaderStatusReading)
            sampleBuffer = [_avStruct->trackOut copyNextSampleBuffer];
        
        if(sampleBuffer)
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
                printf("No video texture cache\n");
                return;
            }
            CVReturn err;
            
            CMTime timestamp = CMSampleBufferGetPresentationTimeStamp( sampleBuffer );
            CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
            
            printf("time: %lld\n",timestamp.value);
            
            width = CVPixelBufferGetWidth(pixelBuffer); 
            height = CVPixelBufferGetHeight(pixelBuffer);
            
            err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, 
                                                               _videoTextureCache,
                                                               pixelBuffer,
                                                               NULL,
                                                               GL_TEXTURE_2D,
                                                               GL_RGBA,
                                                               width,
                                                               height,
                                                               GL_BGRA,
                                                               GL_UNSIGNED_BYTE,
                                                               0,
                                                               &_bgraTexture);

            textureID = CVOpenGLESTextureGetName(_bgraTexture);
            
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
//            CVPixelBufferLockBaseAddress(pixelBuffer, 0);
//            
//            //std::cout<<"captureOutput width: "<<width <<" height: "<<height;
//            
//            
//            glBindTexture(GL_TEXTURE_2D, textureID);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            // This is necessary for non-power-of-two textures
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//            
//            // Using BGRA extension to pull in video frame data directly
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, CVPixelBufferGetBaseAddress(pixelBuffer));
//            
//            CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
            
            CFRelease(sampleBuffer);
        }
    }
    
}