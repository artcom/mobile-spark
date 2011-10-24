// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_mar_Canvas_h_included_
#define _ac_mobile_mar_Canvas_h_included_


#include <masl/numeric_functions.h>
#include <masl/Ptr.h>


namespace mar {

    class Canvas {
        public:
            Canvas();
            ~Canvas();
            void initGLState();
            void preRender(vector4 theClearColor);
        private:
    };

    typedef masl::Ptr<Canvas> CanvasPtr;
};

#endif
