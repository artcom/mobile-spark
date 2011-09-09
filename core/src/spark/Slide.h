#ifndef _included_mobile_spark_Slide_
#define _included_mobile_spark_Slide_

#include "Transform.h"
#include "Text.h"

namespace spark {

    ComponentPtr createSlide(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());

    class SlideImpl : public Transform {
        public: 
            SlideImpl(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
            virtual ~SlideImpl();
            void centerTitles(const int theWidth, const int theHeight);
        private:
            TextPtr titleComponent_;
            TextPtr subtitleComponent_;
    };
    typedef boost::shared_ptr<SlideImpl> SlideImplPtr;
};

#endif
