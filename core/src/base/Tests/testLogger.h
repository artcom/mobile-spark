/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// These coded instructions, statements, and computer programs contain
// proprietary information of ART+COM AG Berlin, and are copy protected
// by law. They may be used, modified and redistributed under the terms
// of GNU General Public License referenced below.
//
// Alternative licensing without the obligations of the GPL is
// available upon request.
//
// GPL v3 Licensing:
//
// This file is part of the ART+COM Y60 Platform.
//
// ART+COM Y60 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// ART+COM Y60 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with ART+COM Y60.  If not, see <http://www.gnu.org/licenses/>.
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
*/
#ifndef _ac_mobile_asl_test_Logger_h_included_
#define _ac_mobile_asl_test_Logger_h_included_

#include "../UnitTest.h"

namespace asl {
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

                //ENSURE_EXCEPTION(Logger::get().log(/*myTime,*/ Severity(SEV_DISABLED+1), "", 0, "Should throw Exception"), asl::Exception);
        }
    };    
};
#endif