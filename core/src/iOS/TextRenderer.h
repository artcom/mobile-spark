#ifndef _included_mobile_ios_TextRenderer_
#define _included_mobile_ios_TextRenderer_

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <CoreText/CoreText.h>
#import <UIKit/UIKit.h>

#include <masl/numeric_functions.h>
#include <masl/Ptr.h>
#include <mar/GLHeaders.h>


namespace ios {
    class TextRenderer {
    public:
        TextRenderer();
        virtual ~TextRenderer();
        void renderText(const std::string & theMessage, int theTextureId, int theFontSize, 
                        vector4 theColor, float theMaxWidth, float theMaxHeight, const std::string & theAlign, const std::string &theFontPath);
        int getTextureID();
        int getTextureWidth();
        int getTextureHeight();
    private:
        GLuint  texture;
        int textureWidth;
        int textureHeight;
        CGSize suggestedSize;
    };
    typedef masl::Ptr<TextRenderer> TextRendererPtr;
};

#endif
