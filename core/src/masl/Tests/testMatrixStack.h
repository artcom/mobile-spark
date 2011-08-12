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
#ifndef _ac_mobile_asl_test_MatrixStack_h_included_
#define _ac_mobile_asl_test_MatrixStack_h_included_

#include "../UnitTest.h"
#include "../MatrixStack.h"

namespace masl {
    class MatrixStack_UnitTest : public UnitTest {
        public:
            MatrixStack_UnitTest() : UnitTest("MatrixStack_UnitTest") {  }
            void run() {
                perform_MatrixStackTest();
            }
            
            void perform_MatrixStackTest() {                
                MatrixStack myStack;
                ENSURE_MSG(myStack.getSize() == 1, "initial matrix stack should be of size 1");
                ENSURE_MSG(myStack.getTop().data()[0] == 1 && 
                           myStack.getTop().data()[1] == 0 &&
                           myStack.getTop().data()[2] == 0 &&
                           myStack.getTop().data()[3] == 0 &&
                           myStack.getTop().data()[4] == 0 &&
                           myStack.getTop().data()[5] == 1 &&
                           myStack.getTop().data()[6] == 0 &&
                           myStack.getTop().data()[7] == 0 &&
                           myStack.getTop().data()[8] == 0 &&
                           myStack.getTop().data()[9] == 0 &&
                           myStack.getTop().data()[10] == 1 &&
                           myStack.getTop().data()[11] == 0 &&
                           myStack.getTop().data()[12] == 0 &&
                           myStack.getTop().data()[13] == 0 &&
                           myStack.getTop().data()[14] == 0 &&
                           myStack.getTop().data()[15] == 1 , "inital top should be identity");
                myStack.push();
                ENSURE_EQUAL(myStack.getSize(), 2);
                ENSURE_MSG(myStack.getTop().data()[0] == 1 && 
                           myStack.getTop().data()[1] == 0 &&
                           myStack.getTop().data()[2] == 0 &&
                           myStack.getTop().data()[3] == 0 &&
                           myStack.getTop().data()[4] == 0 &&
                           myStack.getTop().data()[5] == 1 &&
                           myStack.getTop().data()[6] == 0 &&
                           myStack.getTop().data()[7] == 0 &&
                           myStack.getTop().data()[8] == 0 &&
                           myStack.getTop().data()[9] == 0 &&
                           myStack.getTop().data()[10] == 1 &&
                           myStack.getTop().data()[11] == 0 &&
                           myStack.getTop().data()[12] == 0 &&
                           myStack.getTop().data()[13] == 0 &&
                           myStack.getTop().data()[14] == 0 &&
                           myStack.getTop().data()[15] == 1 , "top should still be identity");
                myStack.translate(23, 24, 25);
                ENSURE_EQUAL(myStack.getSize(), 2);
                //AC_PRINT << myStack.getTop().data()[0] << "  " <<
                //           myStack.getTop().data()[1] << "  " <<
                //           myStack.getTop().data()[2] << "  " <<
                //           myStack.getTop().data()[3] << "  " <<
                //           myStack.getTop().data()[4] << "  " <<
                //           myStack.getTop().data()[5] << "  " <<
                //           myStack.getTop().data()[6] << "  " <<
                //           myStack.getTop().data()[7] << "  " <<
                //           myStack.getTop().data()[8] << "  " <<
                //           myStack.getTop().data()[9] << "  " <<
                //           myStack.getTop().data()[10]<< "  " <<
                //           myStack.getTop().data()[11]<< "  " <<
                //           myStack.getTop().data()[12]<< "  " <<
                //           myStack.getTop().data()[13]<< "  " <<
                //           myStack.getTop().data()[14]<< "  " <<
                //           myStack.getTop().data()[15];
                ENSURE_MSG(myStack.getTop().data()[0] == 1 && 
                           myStack.getTop().data()[1] == 0 &&
                           myStack.getTop().data()[2] == 0 &&
                           myStack.getTop().data()[3] == 0 &&
                           myStack.getTop().data()[4] == 0 &&
                           myStack.getTop().data()[5] == 1 &&
                           myStack.getTop().data()[6] == 0 &&
                           myStack.getTop().data()[7] == 0 &&
                           myStack.getTop().data()[8] == 0 &&
                           myStack.getTop().data()[9] == 0 &&
                           myStack.getTop().data()[10] == 1 &&
                           myStack.getTop().data()[11] == 0 &&
                           myStack.getTop().data()[12] == 23 &&
                           myStack.getTop().data()[13] == 24 &&
                           myStack.getTop().data()[14] == 25 &&
                           myStack.getTop().data()[15] == 1 , "top should still be translation matrix");
                myStack.pop();
                ENSURE_EQUAL(myStack.getSize(), 1);
                myStack.pop();
                ENSURE_EQUAL(myStack.getSize(), 1);


            }
    };    

};
#endif