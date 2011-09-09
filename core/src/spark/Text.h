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
        
        static const char* SPARK_TYPE;             
    private:
        std::string _myText;
        bool _myDirtyFlag;
        int _myFontSize;
        vector4 _myTextColor;
    };

    typedef boost::shared_ptr<Text> TextPtr;
};
#endif 
