#import "TextRenderer.h"
#include <masl/Logger.h>

namespace ios {
    TextRenderer::TextRenderer()
    {
    }

    TextRenderer::~TextRenderer()
    {
    }
    
    void TextRenderer::renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize, 
                                  vector4 theColor, float theMaxWidth, float theMaxHeight, const std::string & theAlign, const std::string &theFontPath, int theLineHeight) 
    {
        CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();

        // Initialize an attributed string.
        NSString *string = [NSString stringWithCString:theMessage.c_str() encoding:NSUTF8StringEncoding];
        //The linebreaks are not encoded by the XML-Parser.
        //Therfore, each "\n" in the string is replaced with an appropriate encoded linebreak, so that Core Text can handle them.
        string = [string stringByReplacingOccurrencesOfString:@"\\n" withString:@"\n"];
        
        CFMutableAttributedStringRef attrString = CFAttributedStringCreateMutable(kCFAllocatorDefault, 0);
        CFAttributedStringReplaceString (attrString, CFRangeMake(0, 0), (CFStringRef)string);
        
        // Create a color and add it as an attribute to the string.
        CGFloat components[] = { theColor[0], theColor[1], theColor[2], theColor[3] };
        CGColorRef color = CGColorCreate(rgbColorSpace, components);
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, CFStringGetLength((CFStringRef)string)), kCTForegroundColorAttributeName, color);
        CFRelease(color);
        
        
        //Create Font and add it as an attribute to the string.
        //Get Font data
        CTFontRef font;
        if (theFontPath.size() != 0){
            NSString *fontPath = [NSString stringWithUTF8String:theFontPath.c_str()];
            NSData *data = [[NSData alloc] initWithContentsOfFile:fontPath];
            CGDataProviderRef fontProvider = CGDataProviderCreateWithCFData((CFDataRef)data);
            [data release];
            
            //Create Core Graphics Font
            CGFontRef theCGFont = CGFontCreateWithDataProvider(fontProvider);
            CGDataProviderRelease(fontProvider);
            
            //Create Core Text Font with Core Graphics Font
            font = CTFontCreateWithGraphicsFont(theCGFont, theFontSize, NULL, NULL);
            CGFontRelease(theCGFont);
        } else {
            //Default Font
            font = CTFontCreateWithName(CFSTR("Helvetica"), theFontSize, NULL);
        }
        //add Font as an attribute to the string
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, CFStringGetLength((CFStringRef)string)), kCTFontAttributeName, font);
        CFRelease(font);
        
        CGFloat strokeStrength = -50/theFontSize; // dirty: increase contrast for small font sizes  
        CFNumberRef stroke = CFNumberCreate (NULL, kCFNumberCGFloatType, &strokeStrength);
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, CFStringGetLength((CFStringRef)string)), kCTStrokeWidthAttributeName, stroke);
        CFRelease(stroke);

        
        //Set alignment
        CTTextAlignment alignment = kCTLeftTextAlignment;
        if (theAlign.compare("center") == 0) {
            alignment = kCTCenterTextAlignment;
        } 
        if (theAlign.compare("left") == 0) {
            alignment = kCTLeftTextAlignment;
        } 
        if (theAlign.compare("right") == 0) {
            alignment = kCTRightTextAlignment;
        }
        
        //Create Paragraph style and set alignment settings to it
        CGFloat lineHeight = theLineHeight;
        //NSLog(@"################################### lineHeight %f",lineHeight);
        CTParagraphStyleSetting styleSetting[] = { {kCTParagraphStyleSpecifierAlignment, sizeof(CTTextAlignment), &alignment} ,
                    { kCTParagraphStyleSpecifierMinimumLineHeight, sizeof(lineHeight), &lineHeight },
                    { kCTParagraphStyleSpecifierMaximumLineHeight, sizeof(lineHeight), &lineHeight }};
        CTParagraphStyleRef paragraphStyle = CTParagraphStyleCreate(styleSetting, sizeof(styleSetting) / sizeof(styleSetting[0]));
        
        //add paragraphStyle as an attribute to the string
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
        renderedGlyphIndex = calcRenderedGlyphIndex(theMessage, fitRange);

        NSLog(@"--------- theMaxHeight: %d and width: %d", theMaxHeight, theMaxWidth);
        
        if (textureWidth != 0 && textureHeight != 0) {
            // Initialize a Bitmap context and set the text matrix to a known value.
            GLubyte *bitmapData = (GLubyte *) calloc((textureWidth * textureHeight * 4), sizeof(GLubyte));
            CGContextRef context = CGBitmapContextCreate(bitmapData, textureWidth, textureHeight, 8, textureWidth * 4, rgbColorSpace, kCGImageAlphaPremultipliedLast);
            CGContextSetRGBFillColor(context, 0, 0, 0, 0);
            
            CGContextSetAllowsAntialiasing(context, YES);
            CGContextSetShouldAntialias(context, YES);
            
            CGContextSetAllowsFontSmoothing(context, YES);
            CGContextSetShouldSmoothFonts(context, YES);
            
            CGContextSetAllowsFontSubpixelPositioning(context, YES);
            CGContextSetShouldSubpixelPositionFonts(context, YES);
            
            CGContextSetAllowsFontSubpixelQuantization(context,YES);
            CGContextSetShouldSubpixelQuantizeFonts(context,YES);
            
            CGContextSetInterpolationQuality(context, kCGInterpolationHigh);

            CGContextSetTextMatrix(context, CGAffineTransformIdentity);
            // Flip the context so that the Bitmap is rendered right side up
            CGContextTranslateCTM(context, 0.0, textureHeight);
            CGContextScaleCTM(context, 1.0, -1.0);
            
            // Initialize a rectangular path.
            CGMutablePathRef path = CGPathCreateMutable();
            CGRect bounds = CGRectMake(0.0, 0.0, textureWidth, textureHeight);
            CGPathAddRect(path, NULL, bounds);
            
            // Create the frame and draw it into the bitmap context
            CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0, 0), path, NULL);
            
            CFRelease(path);
            CFRelease(framesetter); 
            CTFrameDraw(frame, context);
            CFRelease(frame);
            
            //create Opengl Texture
            if (theTextureId == 0)
            {
                glGenTextures(1, &texture);
            } else 
            {
                texture = theTextureId;
            }
            
            glBindTexture(GL_TEXTURE_2D, texture);
            
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)textureWidth, (GLsizei)textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmapData);
            NSLog(@"################# CFRANGE: %d %d size: %d  %s", fitRange.location, fitRange.length, theMessage.size(), theMessage.c_str());
            free(bitmapData);
            CGContextRelease(context);
        } else {
            //create empty Opengl Texture
            if (theTextureId == 0)
            {
                glGenTextures(1, &texture);
            } else 
            {
                texture = theTextureId;
            }
        }
        CGColorSpaceRelease(rgbColorSpace);        
    }
    
    int TextRenderer::getTextureID()
    {
        return texture;
    }
    
    int TextRenderer::getTextureWidth() {
        return textureWidth;
    }
    
    int TextRenderer::getTextureHeight() {
        return textureHeight;
    }
    
    int TextRenderer::calcRenderedGlyphIndex(std::string theMessage, CFRange theFitRange) {
        int myIndex = theFitRange.length;
        size_t found;
        
        found=theMessage.find_first_of("äöüß");
        while (found!=std::string::npos)
        {
            //myIndex--;
            found=theMessage.find_first_of("äöüß",found+1);
        }
        NSLog(@"_______________ new index: %d", myIndex);
        return myIndex;
    }
    
    int TextRenderer::getRenderedGlyphIndex() {
               return renderedGlyphIndex;
    };

    
    
}
