#import "TextRenderer.h"
#include <masl/Logger.h>

namespace ios {
    TextRenderer::TextRenderer()
    {
    }

    TextRenderer::~TextRenderer()
    {
    }
    
    void TextRenderer::renderText(const std::string & theMessage, int theTextureId, int theFontSize, 
                                  vector4 theColor, float theMaxWidth, float theMaxHeight, const std::string & theAlign, const std::string &theFontPath) 
    {
        CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();

        // Initialize an attributed string.
        NSString *string = [NSString stringWithUTF8String:theMessage.c_str()];
        CFMutableAttributedStringRef attrString = CFAttributedStringCreateMutable(kCFAllocatorDefault, 0);
        CFAttributedStringReplaceString (attrString, CFRangeMake(0, 0), (CFStringRef)string);
        
        // Create a color and add it as an attribute to the string.
        CGFloat components[] = { theColor[0], theColor[1], theColor[2], theColor[3] };
        CGColorRef color = CGColorCreate(rgbColorSpace, components);
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, CFStringGetLength((CFStringRef)string)), kCTForegroundColorAttributeName, color);
        CFRelease(color);
        
        //Create Font and add it as an attribute to the string.
        //Get Font data
        NSString *fontPath = [NSString stringWithUTF8String:theFontPath.c_str()];
        NSData *data = [[NSData alloc] initWithContentsOfFile:fontPath];
        CGDataProviderRef fontProvider = CGDataProviderCreateWithCFData((CFDataRef)data);
        [data release];
        
        //Create Core Graphics Font
        CGFontRef theCGFont = CGFontCreateWithDataProvider(fontProvider);
        CGDataProviderRelease(fontProvider);
        
        //Create Core Text Font with Core Graphics Font
        CTFontRef font = CTFontCreateWithGraphicsFont(theCGFont, theFontSize, NULL, NULL);
        CGFontRelease(theCGFont);
        
        //add Font as an attribute to the string
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, CFStringGetLength((CFStringRef)string)), kCTFontAttributeName, font);
        CFRelease(font);
        
        
        /*
        CTParagraphStyleSetting styleSetting[1] = {{kCTParagraphStyleSpecifierAlignment, sizeof(CTTextAlignment), (void*)kCTCenterTextAlignment}};
        //Style
        
        styleSetting[0].value = (void*)kCTCenterTextAlignment;
        
        if (theAlign.compare("center") == 0) {
            styleSetting[0].value = (void*)kCTCenterTextAlignment;
        } 
        if (theAlign.compare("left") == 0) {
            styleSetting[0].value = (void*)kCTLeftTextAlignment;
        } 
        if (theAlign.compare("right") == 0) {
            styleSetting[0].value = (void*)kCTRightTextAlignment;
        }
        styleSetting[0].spec = kCTParagraphStyleSpecifierAlignment;
        styleSetting[0].valueSize = sizeof(CTTextAlignment);
        
        CFIndex index = 1;
        CTParagraphStyleRef paragraphStyle = CTParagraphStyleCreate((const CTParagraphStyleSetting*) &styleSetting, index);
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, CFStringGetLength((CFStringRef)string)), kCTParagraphStyleAttributeName, paragraphStyle);
        
        CFRelease(paragraphStyle);
        */
        
        
        
        
        
        // Create the framesetter with the attributed string.
        CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString(attrString);
        CFRelease(attrString);
        
        //Determine the optimal Texture size
        CFRange fitRange;

        theMaxWidth = theMaxWidth == 0 ? CGFLOAT_MAX : theMaxWidth;
        theMaxHeight = theMaxHeight == 0 ? CGFLOAT_MAX : theMaxHeight;

        suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, 0), NULL, CGSizeMake(theMaxWidth, theMaxHeight), nil);
        textureWidth = ceil(suggestedSize.width);
        textureHeight = ceil(suggestedSize.height);

        
        // Initialize a Bitmap context and set the text matrix to a known value.
        GLubyte *bitmapData = (GLubyte *) calloc((textureWidth * textureHeight * 4.0), sizeof(GLubyte));
        CGContextRef context = CGBitmapContextCreate(bitmapData, textureWidth, textureHeight, 8, textureWidth * 4, rgbColorSpace, kCGImageAlphaNoneSkipLast);

        if (context) {

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
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)textureWidth, (GLsizei)textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmapData);
            
        }
        
        free(bitmapData);
        CGContextRelease(context);
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
}