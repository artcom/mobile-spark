#ifndef _included_mobile_spark_Text_
#define _included_mobile_spark_Text_

#include "ShapeWidget.h"
#include "I18nHandler.h"

namespace spark {
    class Text : public ShapeWidget {
    public:
        Text(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Text();
        virtual void onResume();
        virtual void realize();

        const vector2 & getTextSize();
        void setText(std::string theText) { i18nHandler_->data_ = theText; _myDirtyFlag = true;};

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Text::SPARK_TYPE;};
    protected:
        virtual void build();
    private:

        I18nHandlerPtr i18nHandler_;
        int _myFontSize;
        vector4 _myTextColor;
        vector2 _myTextSize;
        int _myMaxWidth;
        int _myMaxHeight;
        std::string _myFontPath;
        std::string _myTextAlign;
    };

    typedef boost::shared_ptr<Text> TextPtr;
    typedef MemberFunctionEventCallback<Text, TextPtr> TextCB;

};
#endif
