// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#import "TextRenderer.h"
#include <masl/Logger.h>

namespace ios {
    TextRenderer::TextRenderer() {
    }

    TextRenderer::~TextRenderer() {
    }
    
    void TextRenderer::renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize, 
                                  vector4 theColor, float theMaxWidth, float theMaxHeight, const std::string & theAlign, const std::string &theFontPath, int theLineHeight, int theStartIndex) {
        // Initialize an attributed string.
        NSString *string = [NSString stringWithCString:theMessage.c_str() encoding:NSUTF8StringEncoding];
        //The linebreaks are not encoded by the XML-Parser.
        //Therfore, each "\n" in the string is replaced with an appropriate encoded linebreak, so that Core Text can handle them.
        string = [string stringByReplacingOccurrencesOfString:@"\\n" withString:@"\n"];
        CFMutableAttributedStringRef attrString = CFAttributedStringCreateMutable(kCFAllocatorDefault, 0);
        CFAttributedStringReplaceString (attrString, CFRangeMake(0, 0), (CFStringRef)string);

        //Get Font data
        CTFontRef font;
        if (theFontPath.size() != 0){
            NSString *fontPath = [NSString stringWithUTF8String:theFontPath.c_str()];
            NSData *data = [[NSData alloc] initWithContentsOfFile:fontPath];
            CGDataProviderRef fontProvider = CGDataProviderCreateWithCFData((CFDataRef)data);
            [data release];
            //Create Core Text Font with Core Graphics Font
            CGFontRef theCGFont = CGFontCreateWithDataProvider(fontProvider);
            CGDataProviderRelease(fontProvider);
            font = CTFontCreateWithGraphicsFont(theCGFont, theFontSize, NULL, NULL);
            CGFontRelease(theCGFont);
        } else {
            //Default Font
            font = CTFontCreateWithName(CFSTR("Helvetica"), theFontSize, NULL);
        }
        //add Font as an attribute to the string
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, CFStringGetLength((CFStringRef)string)), kCTFontAttributeName, font);
        CFRelease(font);
        
        //Set alignment & paragraph style
        CTTextAlignment alignment = kCTLeftTextAlignment;
        if (theAlign.compare("center") == 0) alignment = kCTCenterTextAlignment;
        if (theAlign.compare("left") == 0) alignment = kCTLeftTextAlignment;
        if (theAlign.compare("right") == 0) alignment = kCTRightTextAlignment;
        CGFloat lineHeight = theLineHeight;
        CTParagraphStyleSetting styleSetting[] = { {kCTParagraphStyleSpecifierAlignment, sizeof(CTTextAlignment), &alignment} ,
                    { kCTParagraphStyleSpecifierMinimumLineHeight, sizeof(lineHeight), &lineHeight },
                    { kCTParagraphStyleSpecifierMaximumLineHeight, sizeof(lineHeight), &lineHeight }};
        CTParagraphStyleRef paragraphStyle = CTParagraphStyleCreate(styleSetting, sizeof(styleSetting) / sizeof(styleSetting[0]));
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, CFAttributedStringGetLength(attrString)), kCTParagraphStyleAttributeName, paragraphStyle);
        CFRelease(paragraphStyle);
        
        // Create the framesetter with the attributed string.
        CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString(attrString);
        CFRelease(attrString);
        CFRange fitRange;
        //Determine the optimal Texture size        
        if (theMaxWidth != 0 && theMaxHeight != 0) {
            suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, 0), NULL, CGSizeMake(theMaxWidth, theMaxHeight), &fitRange);
            textureWidth = ceil(theMaxWidth);
            textureHeight = ceil(theMaxHeight);
        } else if (theMaxWidth == 0 && theMaxHeight == 0) {
            suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, 0), NULL, CGSizeMake(CGFLOAT_MAX, CGFLOAT_MAX), &fitRange);
            textureWidth = ceil(suggestedSize.width);
            textureHeight = ceil(suggestedSize.height);
        } else if (theMaxWidth == 0) {
            suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, 0), NULL, CGSizeMake(CGFLOAT_MAX, theMaxHeight), &fitRange);
            textureWidth = ceil(suggestedSize.width);
            textureHeight = ceil(theMaxHeight);
        } else if ( theMaxHeight == 0){
            suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, 0), NULL, CGSizeMake(theMaxWidth, CGFLOAT_MAX), &fitRange);
            textureWidth = ceil(theMaxWidth);
            textureHeight = ceil(suggestedSize.height);
        } else {
            textureWidth = 0;
            textureHeight = 0;
        }
        
        if (textureWidth != 0 && textureHeight != 0) {
            // Initialize a Bitmap context and set the text matrix to a known value.
            GLubyte *bitmapData = (GLubyte *) calloc((textureWidth * textureHeight), sizeof(GLubyte));
            CGColorSpaceRef grayColorSpace = CGColorSpaceCreateDeviceGray();
            CGContextRef context = CGBitmapContextCreate(bitmapData, textureWidth, textureHeight, 8, textureWidth, grayColorSpace, kCGImageAlphaOnly);
            CGColorSpaceRelease(grayColorSpace);    
            CGContextSetGrayFillColor(context, 1.0f, 1.0f);
            
            CGContextSetAllowsAntialiasing(context, YES);
            CGContextSetShouldAntialias(context, YES);
            CGContextSetAllowsFontSmoothing(context, NO);
            CGContextSetShouldSmoothFonts(context, NO);
            CGContextSetInterpolationQuality(context, kCGInterpolationHigh);

            // Initialize a rectangular path.
            CGMutablePathRef path = CGPathCreateMutable();
            CGRect bounds = CGRectMake(0, 0, textureWidth, textureHeight);
            CGPathAddRect(path, NULL, bounds);
            
            // Create the frame and draw it into the bitmap context
            CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0, 0), path, NULL);
            
            CFRelease(path);
            CFRelease(framesetter); 
            CTFrameDraw(frame, context);
            CFRelease(frame);
            
            // mirror image and colorize alpha map
            GLubyte *buffer2 = (GLubyte *) malloc(textureWidth * textureHeight * 4);
            for(int y = 0; y < textureHeight; y++) {
                for(int x = 0; x < textureWidth; x++) {
                    
                    Byte alpha = bitmapData[y * textureWidth + x] ;
                    
                    buffer2[(textureHeight-1 - y) * textureWidth * 4 + x*4+0] = (Byte) (theColor[0] * alpha);
                    buffer2[(textureHeight-1 - y) * textureWidth * 4 + x*4+1] = (Byte) (theColor[1] * alpha);
                    buffer2[(textureHeight-1 - y) * textureWidth * 4 + x*4+2] = (Byte) (theColor[2] * alpha);
                    buffer2[(textureHeight-1 - y) * textureWidth * 4 + x*4+3] = alpha;
                }
            }
            //create Opengl Texture
            if (theTextureId == 0) {
                glGenTextures(1, &texture);
            } else {
                texture = theTextureId;
            }
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)textureWidth, (GLsizei)textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer2);
            
            free(buffer2);
            free(bitmapData);
            CGContextRelease(context);
        } else {
            //create empty Opengl Texture
            if (theTextureId == 0) {
                glGenTextures(1, &texture);
            } else  {
                texture = theTextureId;
            }
        }
        renderedGlyphIndex = calcRenderedGlyphIndex(theMessage, string, fitRange);
        
        if(renderedGlyphIndex>0 && theMessage[renderedGlyphIndex-1] == '\\') renderedGlyphIndex++;
        if (renderedGlyphIndex!=-1) renderedGlyphIndex += theStartIndex; 
        
    }
    
    int TextRenderer::getTextureID() {
        return texture;
    }
    
    int TextRenderer::getTextureWidth() {
        return textureWidth;
    }
    
    int TextRenderer::getTextureHeight() {
        return textureHeight;
    }
    
    int TextRenderer::calcRenderedGlyphIndex(std::string theMessage, NSString* theString, CFRange theFitRange) {
        NSString * sub = [theString substringToIndex: theFitRange.length];
        std::string myString([sub UTF8String]);
        // search specials:
        unsigned int foundSpecials = 0;
        size_t found;
        found=theMessage.find("\\n");
        while (found!=std::string::npos) {
            if (found < theFitRange.length+foundSpecials) foundSpecials++;
            found=theMessage.find("\\n",found+2);
        }
        if(myString.size()+foundSpecials +1 >= theMessage.size()) {
            return -1;   
        }
        return myString.size() + foundSpecials;        
    }
    
    int TextRenderer::getRenderedGlyphIndex() {
               return renderedGlyphIndex;
    };

    
    
}
