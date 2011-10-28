// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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
        void renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize, 
                        vector4 theColor, float theMaxWidth, float theMaxHeight, const std::string & theAlign, const std::string &theFontPath, int theLineHeight, int theStartIndex);
        int getTextureID();
        int getTextureWidth();
        int getTextureHeight();
        int getRenderedGlyphIndex();
    private:
        int calcRenderedGlyphIndex(std::string theMessage, NSString* theString, CFRange fitRange);

        GLuint  texture;
        int textureWidth;
        int textureHeight;
        int renderedGlyphIndex;
        CGSize suggestedSize;
        
    };
    typedef masl::Ptr<TextRenderer> TextRendererPtr;
};

#endif
