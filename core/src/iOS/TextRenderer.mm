#import "TextRenderer.h"

namespace ios {
    TextRenderer::TextRenderer()
    {
    }

    TextRenderer::~TextRenderer()
    {
    }
    
    void TextRenderer::renderText(const std::string & theMessage, int theTextureId, int theFontSize, 
                                  vector4 theColor, int theMaxWidth, int theMaxHeight) 
    {
        GLubyte *bitmapData = (GLubyte *) calloc((float)(theMaxWidth * theMaxHeight * 4), sizeof(GLubyte));
        CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();
        CGContextRef context = CGBitmapContextCreate(bitmapData, theMaxWidth, theMaxHeight, 8, theMaxWidth * 4, rgbColorSpace, kCGImageAlphaPremultipliedLast);
        
        CGContextSetTextMatrix(context, CGAffineTransformIdentity);
        
        // Initialize a rectangular path.
        CGMutablePathRef path = CGPathCreateMutable();
        CGRect bounds = CGRectMake(0.0, 0.0, (float)theMaxWidth, (float)theMaxHeight);
        CGPathAddRect(path, NULL, bounds);
        
        
        
        // Initialize an attributed string.
        CFStringRef string = (CFStringRef)(theMessage.c_str());
        CFMutableAttributedStringRef attrString = CFAttributedStringCreateMutable(kCFAllocatorDefault, 0);
        CFAttributedStringReplaceString (attrString, CFRangeMake(0, 0), string);
        
        
        
        // Create a color and add it as an attribute to the string.
        CGFloat components[] = { theColor[0], theColor[1], theColor[2], theColor[3] };
        CGColorRef color = CGColorCreate(rgbColorSpace, components);
        CGColorSpaceRelease(rgbColorSpace);
        CFAttributedStringSetAttribute(attrString, CFRangeMake(0, 50), kCTForegroundColorAttributeName, color);
        
        
        
        // Create the framesetter with the attributed string.
        CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString(attrString);
        CFRelease(attrString);
        
        
        
        // Create the frame and draw it into the graphics context
        CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0, 0), path, NULL);
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (float)theMaxWidth, (float)theMaxHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmapData);
        
        free(bitmapData);
        
    }
    
    int TextRenderer::getTestureID()
    {
        return texture;
    }
}