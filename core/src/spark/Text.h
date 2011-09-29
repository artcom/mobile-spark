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
        
        const vector2 & getTextSize();
        void setText(std::string theText) { data_ = theText; _myDirtyFlag = true;};
        void setMaxWidth(int theMaxWidth) {_myMaxWidth = theMaxWidth; _myDirtyFlag = true;};
        void setMaxHeight(int theMaxHeight) {_myMaxHeight = theMaxHeight; _myDirtyFlag = true;};
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
        std::string _myFontPath;
        std::string _myTextAlign;
            
    };

    typedef masl::Ptr<Text> TextPtr;
    typedef MemberFunctionEventCallback<Text, TextPtr> TextCB;

};
#endif
