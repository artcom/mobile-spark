#ifndef _included_mobile_acprojectview_ContentImage_
#define _included_mobile_acprojectview_ContentImage_

#include <spark/Container.h>

namespace acprojectview {
        


    class ContentImage : public spark::Container {
        public: 
            ContentImage(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~ContentImage();
            
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return ContentImage::SPARK_TYPE;};
            
            std::string getSrc();
            
        private:
            std::string _mySrc;
    };
    typedef masl::Ptr<ContentImage> ContentImagePtr;
};

#endif
