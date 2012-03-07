// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_Statistic_
#define _included_mobile_spark_Statistic_

#include <boost/progress.hpp>
#include "Transform.h"

namespace spark {

    class Text;
    typedef masl::Ptr<Text> TextPtr;
        
    class Statistic : public Transform {
    public:
        Statistic(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Statistic();
        virtual void onFrame(EventPtr theEvent);
        virtual void realize();

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Statistic::SPARK_TYPE;};
    private:
        masl::UInt64 lasttime_;
        TextPtr _myFPSText;
        TextPtr _myMemoryText;
        masl::Ptr<boost::timer::timer> _myFPSTimerPtr;                          
    };

    typedef masl::Ptr<Statistic> StatisticPtr;
    typedef masl::WeakPtr<Statistic> StatisticWeakPtr;
    typedef MemberFunctionEventCallback<Statistic, StatisticWeakPtr> StatisticCB;

};
#endif
