#ifndef _included_mobile_acprojectview_Description_
#define _included_mobile_acprojectview_Description_

#include <spark/Container.h>

namespace acprojectview {
        


    class Description : public spark::Container {
        public: 
            Description(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~Description();
            
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return Description::SPARK_TYPE;};
            
            std::string getText() const;
            
        private:
            std::string _myText;
    };
    typedef masl::Ptr<Description> DescriptionPtr;
};

#endif
