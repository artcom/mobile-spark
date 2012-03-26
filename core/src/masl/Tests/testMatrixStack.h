// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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

                myStack.push();
                ENSURE_EQUAL(myStack.getSize(), 2);
                myStack.clear();
                ENSURE_EQUAL(1, myStack.getSize());
            }
    };

};
#endif
