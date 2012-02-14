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

AVAssetReaderTrackOutput* trackOut;
AVAssetReader *assetReader;

namespace ios {
    
    MovieController::MovieController()
    {
        glGenTextures(1, &textureID);
    }
    
    MovieController::~MovieController()
    {
        glDeleteTextures(1, &textureID);
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
                 trackOut = [[AVAssetReaderTrackOutput alloc] initWithTrack:videoTrack outputSettings:outSettings];
                 
                 assetReader = [[AVAssetReader alloc] initWithAsset:asset error:&error];
                 
                 if(!error)
                 {
                     [assetReader addOutput:trackOut];
                     if (![assetReader startReading]) 
                     {
                         NSLog(@"Error: AssetReader could not start reading ...");
                     }
                 }
                 else
                 {
                     NSLog(@"Error: AssetReader could not be initialized ...");
                 }
                 
                 NSLog(@"WE GOT IT LOADED");
                 
             }
             
             
         }];
    }
    
    void MovieController::copyNextFrameToTexture()
    {
        CMSampleBufferRef sampleBuffer = NULL;

        if([assetReader status] == AVAssetReaderStatusReading)
            sampleBuffer = [trackOut copyNextSampleBuffer];
        
        if(sampleBuffer)
        {
            CMTime timestamp = CMSampleBufferGetPresentationTimeStamp( sampleBuffer );
            CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
            
            width = CVPixelBufferGetWidth(pixelBuffer); 
            height = CVPixelBufferGetHeight(pixelBuffer);
            
            CVPixelBufferLockBaseAddress(pixelBuffer, 0);
            
            //AC_TRACE<<"captureOutput width: "<<width <<" height: "<<height;
            
            
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // This is necessary for non-power-of-two textures
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            // Using BGRA extension to pull in video frame data directly
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, CVPixelBufferGetBaseAddress(pixelBuffer));
            
            CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
            
            CFRelease(sampleBuffer);
        }
    }
    
}