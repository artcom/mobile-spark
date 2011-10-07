#ifndef _included_mobile_spark_Text_
#define _included_mobile_spark_Text_

#include "I18nShapeWidget.h"

namespace spark {
    class Text : public I18nShapeWidget {
    public:
        Text(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Text();
        virtual void onResume();
        virtual void realize();
        
        void setText(const std::string & theText) { data_ = theText; _myDirtyFlag = true;};
        void setMaxWidth(int theMaxWidth) {_myMaxWidth = theMaxWidth; _myDirtyFlag = true;};
        void setMaxHeight(int theMaxHeight) {_myMaxHeight = theMaxHeight; _myDirtyFlag = true;};
        void setStartIndex(unsigned int theIndex);

        const vector2 & getTextSize();
        const int getMaxWidth() const { return _myMaxWidth;};
        int getRenderedGlyphIndex();
        int getTotalGlyphCount();

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Text::SPARK_TYPE;};
    protected:
        virtual void build();
    private:
        int _myFontSize;
        vector4 _myTextColor;
        vector2 _myTextSize;
        int _myMaxWidth;
        int _myMaxHeight;
        int _myLineHeight;
        std::string _myFontPath;
        std::string _myTextAlign;
        int _myRenderedGlyphIndex;     
        int _myTotalGlyphCount;     
        int _myTextStartPos; 
    };

    typedef masl::Ptr<Text> TextPtr;
    typedef MemberFunctionEventCallback<Text, TextPtr> TextCB;

};
#endif
