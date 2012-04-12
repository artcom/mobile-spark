// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_mar_BoundingBox_h_included_
#define _ac_mobile_mar_BoundingBox_h_included_

#include <masl/numeric_functions.h>

namespace mar {

    struct BoundingBox {
        vector4 min;
        vector4 max;
        
        bool
        touches2D(const BoundingBox & theOtherBox) const {
            return
                (masl::maximum(theOtherBox.min[0], min[0]) <=
                 masl::minimum(theOtherBox.max[0], max[0])) &&
                (masl::maximum(theOtherBox.min[1], min[1]) <=
                 masl::minimum(theOtherBox.max[1], max[1]));
        }
        bool
        contains2D(const vector2 & p) const {
            return
                (p[0] >= min[0] && p[0] <= max[0] &&
                 p[1] >= min[1] && p[1] <= max[1]);
        }

        std::ostream &
        print(std::ostream & os) const;

        friend std::ostream& operator<<(std::ostream& os, const BoundingBox& bb);
    };
};

#endif

