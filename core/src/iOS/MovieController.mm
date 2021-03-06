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

#include "masl/Logger.h"

namespace ios {
    
    struct AVStruct
    {
        AVAssetReaderTrackOutput *m_videoOut, *m_audioOut;
        AVAssetReader *m_assetReader;
        
        AVPlayer *m_player;
        AVPlayerLayer *m_playerLayer;
        
        AVAudioPlayer *m_audioPlayer;
        
        CMTime m_lastTimestamp;
        double m_timeStamp;
        
        AVStruct(): m_videoOut(NULL),
                    m_audioOut(NULL),
                    m_assetReader(NULL),
                    m_player(NULL),
                    m_playerLayer(NULL),
                    m_audioPlayer(NULL),
                    m_lastTimestamp(CMTimeMake(0,1)),
                    m_timeStamp(0.0){};
        ~AVStruct()
        {
            printf("DESTRUCTOR AVStruct\n");
            
            if(m_videoOut) [m_videoOut release];
            if(m_audioOut) [m_audioOut release];
            if(m_assetReader) [m_assetReader release];
            if(m_player) [m_player release];
            if(m_playerLayer) [m_playerLayer release];
            if(m_audioPlayer) [m_audioPlayer release];
        };
    };
    
    MovieController::MovieController():_bgraTexture(NULL),
    _avStruct(AVStructPtr(new AVStruct)),
    _playing(false)
    {
        glGenTextures(1, &_textureID);
        
        //-- Create CVOpenGLESTextureCacheRef for optimal CVImageBufferRef to GLES texture conversion.
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
                                                    NULL,
                                                    (__bridge void *)[EAGLContext currentContext],
                                                    NULL, &_videoTextureCache);
        if (err) 
        {
            AC_ERROR<<"Error at CVOpenGLESTextureCacheCreate :"<< err;
            return;
        }
    }
    
    MovieController::~MovieController()
    {
        glDeleteTextures(1, &_textureID);
        
        CFRelease(_videoTextureCache);
    }
    
    bool MovieController::isPlaying() const
    {
        return [_avStruct->m_audioPlayer isPlaying];
    }
    
    void MovieController::load(const std::string &filePath)
    {        
        NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:filePath.c_str()]];
        
        //_avStruct->m_player = [[AVPlayer alloc] initWithURL:url];
        
        // create new audio-player instance
        _avStruct->m_audioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:nil];
        
//        _avStruct->m_playerLayer = [[AVPlayerLayer playerLayerWithPlayer:_avStruct->m_player] retain];
//        _avStruct->m_playerLayer.bounds = CGRectMake(0, 0, 480, 320);
        
        AVURLAsset *asset = [AVURLAsset URLAssetWithURL:url options:nil];
        
        NSString *tracksKey = @"tracks";
        
        [asset loadValuesAsynchronouslyForKeys:[NSArray arrayWithObject:tracksKey] 
                             completionHandler:
         ^{
             // completion code
             NSError *error = nil;
             
             //printf("Asset loading complete for: %s\n",[[asset.URL absoluteString] UTF8String]);
             
             //NSArray *tracksArray = [asset tracksWithMediaCharacteristic:AVMediaCharacteristicVisual];
             
             NSArray *videoTrackArray = [asset tracksWithMediaType:AVMediaTypeVideo];
             //NSArray *audioTrackArray = [asset tracksWithMediaType:AVMediaTypeAudio];
             
             _avStruct->m_assetReader = [[AVAssetReader alloc] initWithAsset:asset error:&error];
             
             if([videoTrackArray count])
             {

                 AVAssetTrack *videoTrack = [videoTrackArray objectAtIndex:0];
                 
                 NSMutableDictionary* outSettings = [NSMutableDictionary 
                                                     dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] 
                                                     forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
                 
                 _avStruct->m_videoOut = [[AVAssetReaderTrackOutput alloc] initWithTrack:videoTrack outputSettings:outSettings];
                 
                 if(!error)
                 {
                     [_avStruct->m_assetReader addOutput:_avStruct->m_videoOut];
                 }
                 else
                 {
                     AC_ERROR<<"Error: AssetReader could not add video-track ...";
                 }
             }
             
//             if(false)//[audioTrackArray count])
//             {
//                 
//                 AVAssetTrack *audioTrack = [audioTrackArray objectAtIndex:0];
//                 
//                 NSMutableDictionary* outSettings = nil;
//                 
//                 _avStruct->m_audioOut = [[AVAssetReaderTrackOutput alloc] initWithTrack:audioTrack outputSettings:outSettings];
//                 
//                 if(!error)
//                 {
//                     [_avStruct->m_assetReader addOutput:_avStruct->m_audioOut];
//                 }
//                 else
//                 {
//                     AC_ERROR<<"Error: AssetReader could not add audio-track ...";
//                 }
//             }
             if (![_avStruct->m_assetReader startReading]) 
             {
                 AC_ERROR<<"Error: AssetReader could not start reading ...";
             }

             
         }];
    }
    void MovieController::play()
    {        
        // already playing. nothing to do
        if (_playing) {
            return;
        } // resume paused playback
        else if(_avStruct->m_audioPlayer) {
            AC_INFO << "resume movie playback";
            [_avStruct->m_audioPlayer play];
            _playing = true;
        }
    }
    
    void MovieController::stop()
    {
        // reset all AV related stuff
        _avStruct = AVStructPtr(new AVStruct);
        
        _playing = false;
        
    }
    
    void MovieController::pause()
    {
        //[_avStruct->m_player pause];
        
        if(_playing)
            [_avStruct->m_audioPlayer pause];
        else
            [_avStruct->m_audioPlayer play];
        
        
        _playing = !_playing;
    }
    
    float MovieController::getVolume() const
    {
        return _avStruct->m_audioPlayer.volume;
    }
    
    void MovieController::setVolume(float newVolume)
    {
        float val = newVolume;
        if(val < 0.f) val = 0.f;
        if(val > 1.f) val = 1.f;
        
        _avStruct->m_audioPlayer.volume = val;
        
    }
    
    void MovieController::pixelBufferToGLTexture(const CVPixelBufferRef pixelBuf,GLuint &textureName)
    {
        uint width = CVPixelBufferGetWidth(pixelBuf); 
        uint height = CVPixelBufferGetHeight(pixelBuf);
        
        if (_bgraTexture)
        {
            CFRelease(_bgraTexture);
            _bgraTexture = NULL;
        }
        
        // Periodic texture cache flush every frame
        CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
        
        if (!_videoTextureCache)
        {
            AC_ERROR<<"No video texture cache\n";
            return;
        }
        
        textureName = 0;
        
        CVReturn err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, 
                                                           _videoTextureCache,
                                                           pixelBuf,
                                                           NULL,
                                                           GL_TEXTURE_2D,
                                                           GL_RGBA,
                                                           width,
                                                           height,
                                                           GL_BGRA,
                                                           GL_UNSIGNED_BYTE,
                                                           0,
                                                           &_bgraTexture);
        
        if(err)
        {
            AC_ERROR<<"Error: pixelBufferToGLTexture() failed";
        }
        else
        {
            textureName = CVOpenGLESTextureGetName(_bgraTexture);
        
            glBindTexture(GL_TEXTURE_2D, textureName);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        
    }
    
    void MovieController::pixelBufferToGLTexture_compatibility(const CVPixelBufferRef pixelBuf,GLuint &textureName)
    {
        uint width = CVPixelBufferGetWidth(pixelBuf); 
        uint height = CVPixelBufferGetHeight(pixelBuf);
        
        CVPixelBufferLockBaseAddress(pixelBuf, 0);
        
        //std::cout<<"captureOutput width: "<<width <<" height: "<<height;
        
        
        glBindTexture(GL_TEXTURE_2D, textureName);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // This is necessary for non-power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        // Using BGRA extension to pull in video frame data directly
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, CVPixelBufferGetBaseAddress(pixelBuf));
        
        CVPixelBufferUnlockBaseAddress(pixelBuf, 0);
    }
    
    
    
//    void MovieController::copyNextFrameToTexture()
//    {
//        CALayer *presLayer = [_avStruct->m_playerLayer presentationLayer];
//        
//        _width = _avStruct->m_playerLayer.bounds.size.width;
//        _height = _avStruct->m_playerLayer.bounds.size.height;
//        
//        printf("w:%d -- h:%d\n", _width, _height);
//
//        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
//        
//        uint bytesPerPixel = 4;
//        
//
//        CVPixelBufferRef pixelBuffer = NULL;
//        CVPixelBufferCreate(kCFAllocatorDefault, _width, _height, kCVPixelFormatType_32BGRA, nil, &pixelBuffer);
//        
//        CVPixelBufferLockBaseAddress(pixelBuffer,0);
//        
//        void *pxdata = CVPixelBufferGetBaseAddress(pixelBuffer);
//        
//        CGContextRef context = CGBitmapContextCreate(pxdata,
//                                                     _width,
//                                                     _height,
//                                                     8,
//                                                     _width * bytesPerPixel,
//                                                     colorSpace,
//                                                     kCGImageAlphaPremultipliedLast);
//        
//        CGColorSpaceRelease(colorSpace);
//        
//        if (!context) 
//        {
//            AC_ERROR<<"copyNextFrameToTexture: failed to create context ";
//            return;
//        }
//        
//        [presLayer renderInContext:context];
//        
//        CGContextRelease(context);
//          
//        CVPixelBufferUnlockBaseAddress(pixelBuffer,0);
//        
//        pixelBufferToGLTexture(pixelBuffer, _textureID);
//        
//        //CVPixelBufferRelease(pixelBuffer);
//        CFRelease(pixelBuffer);
//    }
    
    void MovieController::copyNextFrameToTexture()
    {
        CMSampleBufferRef sampleBuffer = NULL;
        
        //CMTime currentTime = _avStruct->m_player.currentTime;
        
        CMTime currentTime = CMTimeMake(_avStruct->m_audioPlayer.currentTime * 1000, 1000) ;

        if([_avStruct->m_assetReader status] == AVAssetReaderStatusReading)
        {
            while (CMTimeCompare(currentTime, _avStruct->m_lastTimestamp) > 0) 
            {
                if(sampleBuffer) CFRelease(sampleBuffer);
                
                sampleBuffer = [_avStruct->m_videoOut copyNextSampleBuffer];
                _avStruct->m_lastTimestamp = CMSampleBufferGetPresentationTimeStamp( sampleBuffer );
            }
        }
        
        if(sampleBuffer)
        {
            CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
            
            _width = CVPixelBufferGetWidth(pixelBuffer); 
            _height = CVPixelBufferGetHeight(pixelBuffer);

            pixelBufferToGLTexture_compatibility(pixelBuffer, _textureID);
            //pixelBufferToGLTexture(pixelBuffer, _textureID);
            
            // do not forget to release the buffer
            CFRelease(sampleBuffer);
        }
        
        else if(! [_avStruct->m_audioPlayer isPlaying])
        {
            AC_PRINT<<"MOVIE FINISHED";
            stop();
            return;
        }
    }
  
}
