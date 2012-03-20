// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_asl_test_Logger_h_included_
#define _ac_mobile_asl_test_Logger_h_included_

#include "../UnitTest.h"

namespace masl {
    class Logger_UnitTest : public UnitTest {
        public:
            Logger_UnitTest() : UnitTest("Logger_UnitTest") {  }
            void run() {
                perform_LoggerTest();
            }
            void perform_LoggerTest() {
                ENSURE_MSG((AC_PRINT << "Logger::Print Test") && true, "Logger Test::print string");
                ENSURE_MSG((AC_PRINT << 5.0f) && true, "Logger Test::print float");
                ENSURE_MSG((AC_PRINT << 5) && true, "Logger Test::print int");
                ENSURE_MSG((AC_PRINT << true) && true, "Logger Test::print boolean");

                ENSURE_MSG((AC_WARNING << "Logger::Warn Test") && true, "Logger Test::warn string");

                //ENSURE_EXCEPTION(Logger::get().log(/*myTime,*/ Severity(SEV_DISABLED+1), "", 0, "Should throw Exception"), masl::Exception);
        }
    };
};
#endif
