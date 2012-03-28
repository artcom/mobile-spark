// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Statistic.h"

#include <masl/proc_functions.h>
#include <masl/string_functions.h>
#include <masl/MobileSDK.h>

#include "BaseApp.h"
#include "Window.h"
#include "Text.h"
#include "SparkComponentFactory.h"

namespace spark {

    const char * const Statistic::SPARK_TYPE = "Statistic";

    Statistic::Statistic(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        Transform(theApp, theXMLNode),
        lasttime_(0)
    {
        _myFPSTimerPtr = masl::Ptr<boost::timer::timer>(new boost::timer::timer);                     
    }

    Statistic::~Statistic() {
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
                "<Text name=\"fps\" y=\"-10\" maxWidth=\"0\" z=\"100\" text=\"fps:\" height=\"-16\" color=\"[1.0,0.0,0.0, 1.0]\" fontsize=\"16\"/>");
        myContainer->addChild(myCreated);
        _myFPSText = boost::static_pointer_cast<spark::Text>(myCreated);

        myCreated = SparkComponentFactory::get().loadSparkComponentsFromString(myContainer->getApp(),
                "<Text name=\"memory_usage\" y=\"-30\" z=\"100\" maxWidth=\"0\" text=\"memory:\" height=\"-16\" color=\"[1.0,0.0,0.0, 1.0]\" fontsize=\"16\"/>");
        myContainer->addChild(myCreated);
        _myMemoryText = boost::static_pointer_cast<spark::Text>(myCreated);

        myStatisticHeight += _myFPSText->getTextSize()[1];

        myCreated = SparkComponentFactory::get().loadSparkComponentsFromString(myContainer->getApp(),
                                                                               "<Text name=\"battery_level\" y=\"-50\" z=\"100\" maxWidth=\"0\" text=\"memory:\" height=\"-16\" color=\"[1.0,0.0,0.0, 1.0]\" fontsize=\"16\"/>");
        myContainer->addChild(myCreated);
        _myBatteryLevelText = boost::static_pointer_cast<spark::Text>(myCreated);
        
        myStatisticHeight += _myFPSText->getTextSize()[1];
        
        setY(myWindow->getSize()[1] - myStatisticHeight);
        setX(10);
    }

    void
    Statistic::onFrame(EventPtr theEvent) {
        StageEventPtr myEvent = boost::static_pointer_cast<StageEvent>(theEvent);
        if (_myFPSText) {
            int myFPS = 1000.0/(_myFPSTimerPtr->elapsed() * 1000);
            _myFPSText->setText("fps: " + masl::as_string(myFPS));
        }
        if (_myMemoryText) {
            unsigned int myMemoryUsage = masl::getUsedMemory();
            unsigned int myTotalMemory = masl::getTotalMemory();
            _myMemoryText->setText("memory: " + masl::as_string(myMemoryUsage) + "/" + masl::as_string(myTotalMemory));
        }
        if (_myBatteryLevelText) {
            float myBatteryLevel = 100.f * masl::MobileSDK_Singleton::get().getNative()->getDeviceBatteryLevel();
            _myBatteryLevelText->setText("battery %: " + masl::as_string(myBatteryLevel) + "/" + masl::as_string(100));
        }
        lasttime_ = myEvent->getCurrentTime();
        _myFPSTimerPtr = masl::Ptr<boost::timer::timer>(new boost::timer::timer);        
    }

}
