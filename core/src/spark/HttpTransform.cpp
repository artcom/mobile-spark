// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "HttpTransform.h"

#include <masl/XMLUtils.h>

#include <animation/AnimationManager.h>
#include <animation/DelayAnimation.h>
#include <animation/SequenceAnimation.h>

#include "SparkComponentFactory.h"
#include "Window.h"


namespace spark {
    const char * const HttpTransform::SPARK_TYPE = "HttpTransform";

    HttpTransform::HttpTransform(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        Transform(theApp, theXMLNode) {
    }

    HttpTransform::~HttpTransform() {
    }

    void
    HttpTransform::realize() {
        Transform::realize();
        setVisible(false);
        HttpTransformPtr ptr = boost::static_pointer_cast<HttpTransform>(shared_from_this());
        WindowPtr myWindow = boost::static_pointer_cast<spark::Window>(getRoot());
        spark::EventCallbackPtr myOnFrameCB = EventCallbackPtr(new HttpTransformEventCB(ptr, &HttpTransform::onFrame));
        myWindow->addEventListener(StageEvent::FRAME, myOnFrameCB);
        url_ = _myXMLNode->getAttributeAs<std::string>("url", "");
        unsigned int loadDelay = _myXMLNode->getAttributeAs<unsigned int>("loadDelay",0);
        unsigned int refreshInterval = _myXMLNode->getAttributeAs<unsigned int>("refreshInterval",0);
        std::string requestModeString = _myXMLNode->getAttributeAs<std::string>("requestMode","GET_IF_NOT_AVAILABLE");
        RequestMode requestMode = (requestModeString == "GET_REPEATING" ? GET_REPEATING :
                                   (requestModeString == "GET_ALWAYS" ? GET_ALWAYS : GET_IF_NOT_AVAILABLE));
        if (requestMode == GET_REPEATING) {
            animation::SequenceAnimationPtr mySequence = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
            animation::DelayAnimationPtr myRequestDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(loadDelay));
            myRequestDelay->setOnFinish(masl::CallbackPtr(new HttpTransformCB(ptr, &HttpTransform::startRequest)));
            animation::DelayAnimationPtr myIntervalDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(refreshInterval));
            myIntervalDelay->setOnFinish(masl::CallbackPtr(new HttpTransformCB(ptr, &HttpTransform::startRequest)));
            myIntervalDelay->setLoop(true);
            mySequence->add(myRequestDelay);
            mySequence->add(myIntervalDelay);
            animation::AnimationManager::get().play(mySequence);
        } else {
            animation::DelayAnimationPtr myRequestDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(loadDelay));
            myRequestDelay->setOnFinish(masl::CallbackPtr(new HttpTransformCB(ptr, &HttpTransform::startRequest)));
            animation::AnimationManager::get().play(myRequestDelay);
        }
    }

    void 
    HttpTransform::startRequest() {
        HttpTransformPtr ptr = boost::static_pointer_cast<HttpTransform>(shared_from_this());
        requestManager_.getRequest(url_, masl::RequestCallbackPtr(
            new HttpTransformRequestCB(ptr, &HttpTransform::onSparkReady)),
            "/downloads/", true, masl::REQUEST_IF_NEWER);
    }

    void
    HttpTransform::onSparkReady(masl::RequestPtr theRequest) {
        AC_DEBUG << "onSparkReady " << theRequest->getResponseString();
        HttpTransformPtr ptr = boost::static_pointer_cast<HttpTransform>(shared_from_this());
        sparkString_ = theRequest->getResponseString();
        std::vector<std::string> assetList = spark::SparkComponentFactory::get().createSrcListFromSpark(sparkString_);
        requestManager_.getAllRequest(masl::getDirectoryPart(url_) + "/assets/", assetList,
            masl::RequestCallbackPtr(),
            masl::RequestCallbackPtr(new HttpTransformRequestCB(ptr, &HttpTransform::onAssetsReady)),
            "/downloads/", true, masl::REQUEST_IF_NOT_AVAILABLE);
    }

    void
    HttpTransform::onAssetsReady(masl::RequestPtr theRequest) {
        AC_DEBUG << "onAssetsReady";
        try {
            ComponentPtr childFromRemote = spark::SparkComponentFactory::get().loadSparkComponentsFromString(getApp(), sparkString_);
            if (_myChildren.size() > 0) {
                _myChildren.clear();
            }
            addChild(childFromRemote);
            setVisible(true);
        } catch(masl::XMLParsingException e) {
            //this can happen when remote file is not completly stored at the time when it is requested
            AC_ERROR << "remote spark string is not parsable, ignore it";
        }
    }

    void
    HttpTransform::onFrame(EventPtr theEvent) {
        if (isRendered()) {
            requestManager_.handleRequests();
        }
    }
}
