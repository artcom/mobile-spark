// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_mar_Viewport_h_included_
#define _ac_mobile_mar_Viewport_h_included_

#include <masl/numeric_functions.h>
#include <masl/Ptr.h>
#include "GlHeaders.h"

namespace mar {

    class Viewport {
        public:
            Viewport();
            Viewport(float theWidth, float theHeight, float theXPos, float theYPos);
            ~Viewport();
            void activate(float theCanvasWidth, float theCanvasHeight);
            vector2 getPos() const { return _myPos; };
            vector2 getSize() const { return _mySize; };
        private:
            vector2 _myPos;
            vector2 _mySize;
    };

    typedef masl::Ptr<Viewport> ViewportPtr;
};

#endif
