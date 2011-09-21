#ifndef _included_mobile_spark_I18nContext_
#define _included_mobile_spark_I18nContext_

#include <masl/Exception.h>

#include "Container.h"
#include "BaseApp.h"
#include "I18nConstants.h"

namespace spark {

    ///////////////////////////////////////////////////////I18nContext
    class I18nContext : public Container {
    public: 
        I18nContext(const spark::BaseAppPtr& theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~I18nContext();

        LANGUAGE getLanguage() const { return language_;};
        virtual void switchLanguage(const LANGUAGE theLanguage);
        virtual void addChild(const ComponentPtr theChild, const bool theSetParentFlag = true);
        virtual void setup();

        static const char * const SPARK_TYPE;             
        virtual const char * const & getType() const { return I18nContext::SPARK_TYPE;};

    private:
        LANGUAGE  language_;
        LANGUAGE  defaultLanguage_;
        bool needsUpdate_;
    };
    typedef boost::shared_ptr<I18nContext> I18nContextPtr;

    ///////////////////////////////////////////////////////I18nItem
    class I18nItem : public Component {
    public:
        I18nItem(const spark::BaseAppPtr& theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~I18nItem();
        virtual void switchLanguage(const LANGUAGE theLanguage);
        std::string getLanguageData(const LANGUAGE theLanguage = NO_LANGUAGE) const;
    private:
        LANGUAGE  language_;
        std::map<LANGUAGE, std::string> languageData_;  //maps language key to language specific string
    };
    typedef boost::shared_ptr<I18nItem> I18nItemPtr;
    DEFINE_EXCEPTION(I18nItemNotFoundException, Exception);

    ///////////////////////////////////////////////////////I18nText
    class I18nText : public I18nItem {
    public:
        I18nText(const spark::BaseAppPtr & theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~I18nText();

        static const char * const SPARK_TYPE;             
        virtual const char * const & getType() const { return I18nText::SPARK_TYPE;};
    };
    typedef boost::shared_ptr<I18nText> I18nTextPtr;

    ///////////////////////////////////////////////////////I18nText
    class I18nImage : public I18nItem {
    public:
        I18nImage(const spark::BaseAppPtr & theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~I18nImage();

        static const char * const SPARK_TYPE;             
        virtual const char * const & getType() const { return I18nImage::SPARK_TYPE;};
    };
    typedef boost::shared_ptr<I18nImage> I18nImagePtr;
};
#endif 
