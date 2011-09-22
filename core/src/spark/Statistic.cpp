#include "Statistic.h"
#include "BaseApp.h"

#include "SparkComponentFactory.h"

namespace spark {

    const char * const Statistic::SPARK_TYPE = "Statistic";

    Statistic::Statistic(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        Transform(theApp, theXMLNode, theParent) {

    }

    Statistic::~Statistic() {
    }

    void
    Statistic::onPause() {
        Transform::onPause();
    }

    void
    Statistic::realize() {
        Transform::realize();
        WindowPtr myWindow = boost::static_pointer_cast<spark::Window>(getRoot());
        StatisticPtr ptr = boost::static_pointer_cast<Statistic>(shared_from_this());
        spark::EventCallbackPtr myFrameCB = EventCallbackPtr(new StatisticCB(ptr, &Statistic::onFrame));
        myWindow->addEventListener(StageEvent::FRAME, myFrameCB);

        int myStatisticHeight = 0;

        ContainerPtr myContainer = boost::static_pointer_cast<spark::Container>(shared_from_this());
        ComponentPtr myCreated = SparkComponentFactory::get().loadSparkComponentsFromString(myContainer->getApp(),
                "<Text name=\"fps\" y=\"-10\" maxWidth=\"0\" text=\"fps:\" height=\"-16\" color=\"[1.0,0.0,0.0, 1.0]\" fontsize=\"16\"/>");
        myContainer->addChild(myCreated);
        _myFPSText = boost::static_pointer_cast<spark::Text>(myCreated);

        myStatisticHeight += _myFPSText->getTextSize()[1];
        setY(myWindow->getSize()[1] - myStatisticHeight);
        setX(10);
    }

    void
    Statistic::onFrame(EventPtr theEvent) {
        StageEventPtr myEvent = boost::static_pointer_cast<StageEvent>(theEvent);
        if (_myFPSText) {
            int myFPS = 1000.0/(myEvent->getCurrentTime() - lasttime_);
            _myFPSText->setText("fps: " + as_string(myFPS));
        }
        lasttime_ = myEvent->getCurrentTime();
    }

}
