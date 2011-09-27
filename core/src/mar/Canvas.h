#ifndef _ac_mobile_mar_Canvas_h_included_
#define _ac_mobile_mar_Canvas_h_included_

#include "GlHeaders.h"

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
