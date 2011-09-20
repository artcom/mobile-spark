#ifndef _included_masl_Plane_
#define _included_masl_Plane_

#include "numeric_functions.h"

namespace masl {

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

}
#endif

