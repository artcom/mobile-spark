#include "HttpTransform.h"

#include <animation/AnimationManager.h>
#include <animation/DelayAnimation.h>

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
        HttpTransformPtr ptr = boost::static_pointer_cast<HttpTransform>(shared_from_this());
        WindowPtr myWindow = boost::static_pointer_cast<spark::Window>(getRoot());
        spark::EventCallbackPtr myOnFrameCB = EventCallbackPtr(new HttpTransformEventCB(ptr, &HttpTransform::onFrame));
        myWindow->addEventListener(StageEvent::FRAME, myOnFrameCB);
        url_ = _myXMLNode->getAttributeAs<std::string>("url", "");
        setVisible(false);
        animation::DelayAnimationPtr myRequestDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(0));
        myRequestDelay->setOnFinish(masl::CallbackPtr(new HttpTransformCB(ptr, &HttpTransform::startRequest)));
        animation::AnimationManager::get().play(myRequestDelay);
    }

    void HttpTransform::startRequest() {
        HttpTransformPtr ptr = boost::static_pointer_cast<HttpTransform>(shared_from_this());
        requestManager_.getRequest(url_, masl::RequestCallbackPtr(
            new HttpTransformRequestCB(ptr, &HttpTransform::onSparkReady)),
            "/downloads/", true, masl::REQUEST_IF_NOT_AVAILABLE);
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
        ComponentPtr myNewSparkComponent = 
            spark::SparkComponentFactory::get().loadSparkComponentsFromString(getApp(), sparkString_);
        addChild(myNewSparkComponent);
        setVisible(true);
    }

    void
    HttpTransform::onFrame(EventPtr theEvent) {
        requestManager_.handleRequests();
    }
}
