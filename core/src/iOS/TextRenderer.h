#ifndef _included_mobile_ios_TextRenderer_
#define _included_mobile_ios_TextRenderer_

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <CoreText/CoreText.h>
#import <UIKit/UIKit.h>


#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/numeric_functions.h>
#include <mar/GLHeaders.h>


namespace ios {
    class TextRenderer {
    public:
        TextRenderer();
        virtual ~TextRenderer();
        void renderText(const std::string & theMessage, int theTextureId, int theFontSize, 
                        vector4 theColor, int theMaxWidth, int theMaxHeight);
        int getTextureID();
        
    private:
        GLuint  texture;
    };
    typedef boost::shared_ptr<TextRenderer> TextRendererPtr;
};

#endif