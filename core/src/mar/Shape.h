#ifndef _ac_mobile_mar_Shape_h_included_
#define _ac_mobile_mar_Shape_h_included_

#include <vector>
#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/MatrixStack.h>

#include "Element.h"


namespace mar {

    class Shape {
    public:
        std::vector<ElementPtr> elementList; 

        virtual void render(const matrix & theMvp) const;
    };

    typedef boost::shared_ptr<Shape> ShapePtr;
};

#endif 
