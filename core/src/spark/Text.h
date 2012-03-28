// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_Text_
#define _included_mobile_spark_Text_

#include "I18nShapeWidget.h"

namespace spark {
    
    typedef std::map<unsigned int, int> RenderedGlyphTextureMap;    
    class TextGlyphIndexMap : public masl::Singleton<TextGlyphIndexMap> {
        public:
            TextGlyphIndexMap();
            virtual ~TextGlyphIndexMap();
            void store(const unsigned int theKey, int myRenderedGlyphIndex);
            int getIndex(const unsigned int theKey);
        private:
            RenderedGlyphTextureMap _myRenderedGlyphTextureMap;
    };
    
    class Text : public I18nShapeWidget {
    public:
        Text(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Text();
        
        void setText(const std::string & theText) { setI18nData(theText); _myDirtyFlag = true;};
        inline const std::string & getText() const { return getData();};
        
        void setColor(const vector4 &theColor);
        
        void setFont(const std::string &theFont);
        
        void setMaxWidth(int theMaxWidth) {_myMaxWidth = theMaxWidth; _myDirtyFlag = true;};
        void setMaxHeight(int theMaxHeight) {_myMaxHeight = theMaxHeight; _myDirtyFlag = true;};
        void setStartIndex(unsigned int theIndex);

        const vector2 & getTextSize();
        const int getMaxWidth() const { return _myMaxWidth;};
        int getRenderedGlyphIndex();

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Text::SPARK_TYPE;};
        virtual std::string getAttributesAsString() const;
    protected:
        virtual void build();
    private:
        static unsigned ourTextCounter;
        int _myFontSize;
        vector4 _myTextColor;
        vector2 _myTextSize;
        int _myMaxWidth;
        int _myMaxHeight;
        int _myLineHeight;
        std::string _myFontPath;
        std::string _myTextAlign;
        int _myRenderedGlyphIndex;     
        int _myTextStartPos; 
        bool _myCacheFlag;
    };

    typedef masl::Ptr<Text> TextPtr;
    typedef MemberFunctionEventCallback<Text, TextPtr> TextCB;

};
#endif
