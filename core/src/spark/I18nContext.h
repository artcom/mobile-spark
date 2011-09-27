#ifndef _included_mobile_spark_I18nContext_
#define _included_mobile_spark_I18nContext_

#include <masl/Exception.h>

#include "Container.h"
#include "I18nConstants.h"

namespace spark {
    DECLARE_EXCEPTION(I18nItemNotFoundException, masl::Exception);

    class BaseApp;
    typedef masl::Ptr<BaseApp> BaseAppPtr;

    ///////////////////////////////////////////////////////I18nContext
    class I18nContext : public Container {
    public: 
        I18nContext(const spark::BaseAppPtr& theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~I18nContext();

        LANGUAGE getLanguage() const { return language_;};
        virtual void switchLanguage(const LANGUAGE theLanguage);
        virtual void addChild(const ComponentPtr theChild);
        inline void setup() {
            switchLanguage(defaultLanguage_);
        };


        static const char * const SPARK_TYPE;             
        virtual const char * const & getType() const { return I18nContext::SPARK_TYPE;};

    private:
        LANGUAGE  language_;
        LANGUAGE  defaultLanguage_;
        bool needsUpdate_;
    };
    typedef masl::Ptr<I18nContext> I18nContextPtr;

    ///////////////////////////////////////////////////////I18nItem
    class I18nItem : public Component {
    public:
        I18nItem(const spark::BaseAppPtr& theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~I18nItem();
        virtual void switchLanguage(const LANGUAGE theLanguage);
        std::string getLanguageData(const LANGUAGE theLanguage = NO_LANGUAGE) const;
        static const char * const SPARK_TYPE;             
        virtual const char * const & getType() const { return I18nItem::SPARK_TYPE;};
    private:
        LANGUAGE  language_;
        std::map<LANGUAGE, std::string> languageData_;  //maps language key to language specific string
    };
    typedef masl::Ptr<I18nItem> I18nItemPtr;
};
#endif 
