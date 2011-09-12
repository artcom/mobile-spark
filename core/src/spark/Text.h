#ifndef _included_mobile_spark_Text_
#define _included_mobile_spark_Text_

#include "ShapeWidget.h"

using namespace mar;

namespace spark {
    ComponentPtr createText(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    class Text : public ShapeWidget {
    public: 
        Text(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Text();    
        virtual void prerender(MatrixStack& theCurrentMatrixStack);   
        virtual void onResume();
        const vector2 & getTextSize() const { return _myTextSize; }
        static const char* SPARK_TYPE;             
    private:
        void build();
        
        std::string _myText;
        bool _myDirtyFlag;
        int _myFontSize;
        vector4 _myTextColor;
        vector2 _myTextSize;
        int _myMaxHeight;
        int _myMaxWidth;
    };

    typedef boost::shared_ptr<Text> TextPtr;
};
#endif 
