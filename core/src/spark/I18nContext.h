// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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
        LANGUAGE getDefaultLanguage() const { return defaultLanguage_;};
        void switchLanguage(const LANGUAGE theLanguage);
        virtual void addChild(const ComponentPtr theChild);
        virtual void realize();        

        static const char * const SPARK_TYPE;             
        virtual const char * const & getType() const { return I18nContext::SPARK_TYPE;};
        virtual std::string getAttributesAsString() const;

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
