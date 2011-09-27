#ifndef _included_mobile_demoapp_Slide_
#define _included_mobile_demoapp_Slide_

#include <spark/Transform.h>
#include <spark/Text.h>

namespace demoapp {

    class SlideImpl : public spark::Transform {
        public: 
            SlideImpl(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~SlideImpl();
            void centerTitles(const int theWidth, const int theHeight);
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return SlideImpl::SPARK_TYPE;};
        private:
            spark::TextPtr titleComponent_;
            spark::TextPtr subtitleComponent_;
    };
    typedef masl::Ptr<SlideImpl> SlideImplPtr;
};

#endif
