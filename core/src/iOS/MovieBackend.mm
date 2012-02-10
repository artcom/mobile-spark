// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "MovieBackend.h"

namespace ios 
{
    
    MovieBackend::MovieBackend()
    {
        
    }

    MovieBackend::~MovieBackend()
    {
        
    }
    
    void MovieBackend::playMovie(const std::string &theURL)
    {
        NSURL *url = [NSURL URLWithString:[NSString stringWithUTF8String:theURL.c_str()]];
        AVURLAsset *asset = [AVURLAsset assetWithURL:url];
        
        NSString *tracksKey = @"tracks";
        
        [asset loadValuesAsynchronouslyForKeys:[NSArray arrayWithObject:tracksKey] 
                             completionHandler:
         ^{
             // completion code
             printf("Asset loading complete for: %s",theURL.c_str());
             
             //NSArray *tracksArray = [asset tracksWithMediaCharacteristic:AVMediaCharacteristicVisual];
             NSArray *tracksArray = [asset tracksWithMediaType:AVMediaTypeVideo];
             
             if([tracksArray count])
             {
                 NSError *error = nil;
                 
                 AVAssetTrack *videoTrack = [tracksArray objectAtIndex:0];
                 
                 NSMutableDictionary* outSettings = [NSMutableDictionary 
                                                    dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] 
                                                    forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
                 
                 AVAssetReaderTrackOutput *trackOutput = 
                 [[AVAssetReaderTrackOutput alloc] initWithTrack:videoTrack outputSettings:outSettings];
                 
                 AVAssetReader *assetReader = [[AVAssetReader alloc] initWithAsset:asset error:&error];
                 
                 if(!error)
                 {
                     [assetReader addOutput:trackOutput];
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
                 
                 CMSampleBufferRef sampleBuffer = [trackOutput copyNextSampleBuffer];
                 
                 CMTime timestamp = CMSampleBufferGetPresentationTimeStamp( sampleBuffer );
                 CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
                 
                 int width = CVPixelBufferGetWidth(pixelBuffer); 
                 int height = CVPixelBufferGetHeight(pixelBuffer);
             }
             
             
         }];
    }
}