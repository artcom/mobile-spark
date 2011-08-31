/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// This file is part of the ART+COM Standard Library (asl).
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
//
// Description: TODO
//
// Last Review: NEVER, NOONE
//
//  review status report: (perfect, ok, fair, poor, disaster, notapplicable, unknown)
//    usefullness            : unknown
//    formatting             : unknown
//    documentation          : unknown
//    test coverage          : unknown
//    names                  : unknown
//    style guide conformance: unknown
//    technical soundness    : unknown
//    dead code              : unknown
//    readability            : unknown
//    understandabilty       : unknown
//    interfaces             : unknown
//    confidence             : unknown
//    integration            : unknown
//    dependencies           : unknown
//    cheesyness             : unknown
//
//    overall review status  : unknown
//
//    recommendations:
//       - unknown
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/
//
//    $RCSfile: Plane.h,v $
//
//     $Author: danielk $
//
//   $Revision: 1.17 $
//
// Description: Plane class
//
//=============================================================================

#ifndef _included_masl_Plane_
#define _included_masl_Plane_

//#include "asl_math_settings.h"

//#include "vector3234.h"
//#include <cml/vector3.h>
#include "numeric_functions.h"

namespace masl {

    /*! @addtogroup aslmath */
    /* @{ */

    // Note: use Triangle to construct a plane from three points
	class Plane {
	public:
        // Note: offset is the negative distance of the plane from the origin; it is the float you
        // have to add to move the plane into the origin.
		Plane(){};
        Plane(const Plane & p)
            : normal(p.normal), offset(p.offset) {}
		Plane(const vector3 & theNormal, float theOffset)
            : normal(theNormal), offset(theOffset) {}
        Plane(const vector3 & theNormal, const vector3 & thePointOnPlane) :
            normal(theNormal)
        {
            vector3 myProduct = normal;
            myProduct[0] = normal[0] * thePointOnPlane[0];
            myProduct[1] = normal[1] * thePointOnPlane[1];
            myProduct[2] = normal[2] * thePointOnPlane[2];
            offset = - (myProduct[0] + myProduct[1] + myProduct[2]);
        }
        void normalize() {
            normal.normalize();
            //normal = normalized(normal);
        }
        void setNormal(const vector3 & theNormal) {
            normal = theNormal;
        }
        void setOffset(float theOffset) {
            offset = theOffset;
        }

        Plane & operator=(const Plane & otherPlane) {
            if (this != & otherPlane) {
                normal = otherPlane.normal;
                offset = otherPlane.offset;
            }
            return * this;
        }

        vector4
        getCoefficients() const {
            return vector4(normal[0], normal[1], normal[2], offset);
        }

    public:
		vector3 normal; // a vector3 perpendicular to the plane
		float offset;  // distance from the origin
	};

	inline vector3 projection(const vector3 & d, const Plane & j) {
		return d - (dot(d,j.normal) * j.normal);
	}

	//nearest point to p on a plane p
	inline vector3 nearest(const vector3 & p, const Plane & j) {
		return  p - ((j.offset + dot(j.normal,vector3(p)))/dot(j.normal,j.normal)) * j.normal;
	}
	inline vector3 nearest(const Plane & j, const vector3 & p) {
        return nearest(p, j);
    }

	inline float distance(const vector3 & p, const Plane & j) {
		return distance(p, nearest(p,j));
	}

	inline float signedDistance(const vector3 & p, const Plane & j) {
		return dot(vector3(p), j.normal) + j.offset;
	}

	inline float distance(const Plane & j, const vector3 & p) {
		return distance(p, j);
	}

//    typedef Plane Planef;
//    typedef Plane<double> Planed;
}
#endif

