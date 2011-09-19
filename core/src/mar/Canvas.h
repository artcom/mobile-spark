#ifndef _ac_mobile_mar_Canvas_h_included_
#define _ac_mobile_mar_Canvas_h_included_

#include "GlHeaders.h"
#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/numeric_functions.h>


namespace mar {

    class Canvas {
        public:
            Canvas();
            ~Canvas();
            void initGLState();
            void preRender(vector4 theClearColor);
        private:
    };

    typedef boost::shared_ptr<Canvas> CanvasPtr;
};

#endif
