#ifndef _included_mobile_demoapp_Slide_
#define _included_mobile_demoapp_Slide_

#include <spark/Transform.h>
#include <spark/Text.h>

namespace demoapp {

    class SlideImpl : public spark::Transform {
        public: 
            SlideImpl(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode, spark::ComponentPtr theParent);
            virtual ~SlideImpl();
            void centerTitles(const int theWidth, const int theHeight);
        private:
            spark::TextPtr titleComponent_;
            spark::TextPtr subtitleComponent_;
    };
    typedef boost::shared_ptr<SlideImpl> SlideImplPtr;
};

#endif
