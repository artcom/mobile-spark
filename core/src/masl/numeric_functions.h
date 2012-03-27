// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_masl_numeric_functions_
#define _included_masl_numeric_functions_

#include <cml/cml.h>
#include <cml/util.h>
#include <cml/mathlib/typedef.h>
#include <cml/mathlib/misc.h>
#include <cml/vector/vector_ops.h>

typedef cml::constantsf constants;
typedef cml::matrix44f_c matrix;
typedef cml::vector4f vector4;
typedef cml::vector3f vector3;
typedef cml::vector2f vector2;
typedef cml::quaternionf quaternion;

#include <cmath>
#include <cstdlib>
#include <limits>
#include <string>

namespace masl {

inline float distance(const vector3 & a, const vector3 & b) {
    return (b - a).length();
}

template<class T>
inline
T abs(T theNumber) {
    return (theNumber < (T)(0) ? -theNumber : theNumber);
}

template<class T>
inline
int round(T theNumber) {
    if ((theNumber - floor(theNumber)) >= (T)(0.5)) {
        return int(ceil(theNumber));
    } else {
        return int(floor(theNumber));
    }
}

/** Returns true if a and b are almost equal
 *
 * @retval true if @f[|a - b| \leq (1 + |a| + |b|) * \varepsilon @f]
 * @retval false otherwise
 *
 * @param a
 * @param b
 * @param thePrecision @f$ \varepsilon @f$
 */
inline
bool almostEqual(double a, double b, double thePrecision = 1E-5) {
    return (fabs(a - b) <= ((1 + fabs(a) + fabs(b)) * thePrecision ) );
}

inline
float distance(float a, float b) {
    return fabsf(a - b);
}

inline
double distance(double a, double b) {
    return fabs(a - b);
}

template <class NUM>
NUM minimum(NUM a, NUM b) {
    if (a < b)
        return a;
    else
        return b;
}

template <class NUM>
NUM maximum(NUM a, NUM b) {
    if (a > b)
        return a;
    else
        return b;
}

template <class NUM>
inline bool is_power_of_2(NUM n) {
    return (n & (n - 1)) == 0;
}

template <class NUM>
inline NUM next_power_of_2(NUM n) {
    n -= 1;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

inline long
squareroot(long n) {
    long r    = 1;
    long rnew = 1;
    long rold = r;
    do {
        rold = r;
        r = rnew;
        rnew = ( r + ( n / r));
        rnew >>=1;
    } while (rold != rnew);
    return rnew;
}

inline bool
isPrime(long n) {
    long sn = squareroot(n);
    for (long i = 2; i <= sn; ++i) {
        if ( n % i == 0) {
            return false;
        }
    }
    return true;
}


// Functor used to tidy up vectors and matrixes.
template <class T>
class tidy {
    public:
        void operator ()(T& num, T epsilon=1e-12) {
            if (fabs(num) < epsilon) {
                num = 0;
            }
            if (fabs(num-1)<epsilon) {
                num = 1;
            }
            if (fabs(num+1)<epsilon) {
                num = -1;
            }
        }
};

// Shifts an angle into a range between theMin and theMax
template <class Number>
Number shiftToRange(Number theAngle, double theMin = -constants::pi(), double theMax = constants::pi()) {
    double myResult = theAngle;
    if (myResult < theMax) {
        while (myResult < theMin) {
            myResult += constants::two_pi();
        }
    } else {
        while (myResult > theMax) {
            myResult -= constants::two_pi();
        }
    }
    return Number(myResult);
}

// Specializations for integer types
inline
int interpolate(const int & a, const int & b, const double & theAlpha) {
    double myValue = double(a) + double(b - a) * theAlpha;
    if ((myValue - int(myValue)) <= 0.5) {
        return int(myValue);
    } else {
        return int(ceil(myValue));
    }
}

inline
unsigned interpolate(const unsigned & a, const unsigned & b, const double & theAlpha) {
    double myValue = double(a) + double(b - a) * theAlpha;
    if ((myValue - unsigned(myValue)) <= 0.5) {
        return unsigned(myValue);
    } else {
        return unsigned(ceil(myValue));
    }
}

inline
bool & interpolate(bool & a, bool & b, const double & theAlpha) {
    if (theAlpha <= 0.5) {
        return a;
    } else {
        return b;
    }
}

template <class T>
T interpolate(T & a, T & b, const double & theAlpha) {
    return static_cast<T>(a + (b - a) * theAlpha);
}
template <class T>
T interpolate(const T & a, const T & b, const double & theAlpha) {
    return static_cast<T>(a + (b - a) * theAlpha);
}

#undef min
#undef max


template <bool IsSigned, bool IsInteger>
struct NumberSignedIntegerTraits{};

template<> struct NumberSignedIntegerTraits<true,true> {
    template <class T>
    static
    T min(T max) {
        return -max - 1;
    }
};
template<> struct NumberSignedIntegerTraits<true,false> {
    template <class T>
    static
    T min(T max) {
        return -max;
    }
};
template<> struct NumberSignedIntegerTraits<false,true> {
    template <class T>
    static
    T min(T /*max*/) {
        return 0;
    }
};
template<> struct NumberSignedIntegerTraits<false,false> {
    template <class T>
    static
    T min(T /*max*/) {
        return 0;
    }
};

template <class T>
struct NumericTraits {
    static
    T min() {
        return NumberSignedIntegerTraits<
            std::numeric_limits<T>::is_signed,
            std::numeric_limits<T>::is_integer
        >::min(std::numeric_limits<T>::max());
    }
    static
    T max() {
        return std::numeric_limits<T>::max();
    }
};


// return the roots for the quadratic equation ax^2 + bx + C = 0
template <class Number>
int solveQuadratic(Number a, Number b, Number c, Number & theRoot0, Number & theRoot1) {
    if (a==0) {
        if (b==0) {
            if (c==0) {
                theRoot0 = 0;
                theRoot1 = 0;
                return 1;
            } else {
                return 0;
            }
        }
        theRoot0 = -c/b;
        theRoot1 = -c/b;
        return 1;
    }

    Number myDet = b * b - Number(4) * a * c;
    if (myDet < Number(0)) {
        return 0;
    }
    Number myDetSqrt = sqrt(myDet);
    theRoot0 = (-b - myDetSqrt) / (2*a);
    theRoot1 = (-b + myDetSqrt) / (2*a);
    if (almostEqual(myDet,0)) {
        return 1;
    }
    return 2;
}

/**
 * bi-cubic interpolation between in/out
 * @param theValue Value to interpolate.
 * @param theIn In-point for smoothstep; default is 0.
 * @param theOut Out-point for smoothstep; default is 1.
 * @return Interpolated value.
 */
template <class T>
T smoothStep(T theValue, T theIn = 0, T theOut = 1)
{
    if (theValue <= theIn) {
        return 0;
    } else if (theValue >= theOut) {
        return 1;
    }

    T myOutput = (theValue - theIn) / (theOut - theIn);
    return (myOutput * myOutput) * (3 - 2 * myOutput);
}

template <class NUMBER>
NUMBER ipow(NUMBER a, int x) {
    if (x == 0) return 1;
    NUMBER myResult = a;
    while (--x) myResult *= a;
    return myResult;
}

template <class NUMBER>
NUMBER random(NUMBER min, NUMBER max) {
    return static_cast<NUMBER>(min + rand()/double(RAND_MAX) * (max - min));
}

} //Namespace masl

#endif
