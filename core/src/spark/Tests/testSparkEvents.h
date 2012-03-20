// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_spark_test_SparkEvents_h_included_
#define _ac_mobile_spark_test_SparkEvents_h_included_

#include <string>
#include <masl/UnitTest.h>

namespace spark {
    class SparkEvents_UnitTest : public UnitTest {
    public:
        SparkEvents_UnitTest() : UnitTest("SparkEvents_UnitTest") {  }
        void run() {
            perform_TouchEventTest();
            perform_StageEventTest();
        }

        void perform_StageEventTest() {
        }

        void perform_TouchEventTest() {
        }

    };
};
#endif
