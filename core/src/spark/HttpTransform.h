// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_HttpTransform_
#define _included_mobile_spark_HttpTransform_

#include "Transform.h"

#include <masl/RequestManager.h>

namespace spark {

    enum RequestMode {GET_ALWAYS, GET_REPEATING, GET_IF_NOT_AVAILABLE};
    class HttpTransform : public Transform {
    public:
        HttpTransform(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~HttpTransform();
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return HttpTransform::SPARK_TYPE;};

        virtual void realize();
        virtual void onFrame(EventPtr theEvent);
        void startRequest();
    protected:
        masl::RequestManager requestManager_;
    private:
        void onSparkReady(masl::RequestPtr theRequest);
        void onAssetsReady(masl::RequestPtr theRequest);
        std::string url_;
        std::string sparkString_;
    };

    typedef masl::Ptr<HttpTransform> HttpTransformPtr;
    typedef masl::WeakPtr<HttpTransform> HttpTransformWeakPtr;
    typedef masl::MemberFunctionCallback<HttpTransform, HttpTransformWeakPtr> HttpTransformCB;
    typedef masl::MemberFunctionRequestCallback<HttpTransform, HttpTransformWeakPtr> HttpTransformRequestCB;
    typedef MemberFunctionEventCallback<HttpTransform, HttpTransformWeakPtr> HttpTransformEventCB;
};

#endif
