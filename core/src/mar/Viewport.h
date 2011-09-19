#ifndef _ac_mobile_mar_Viewport_h_included_
#define _ac_mobile_mar_Viewport_h_included_

#include "GlHeaders.h"
#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/numeric_functions.h>

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

    typedef boost::shared_ptr<Viewport> ViewportPtr;
};

#endif
