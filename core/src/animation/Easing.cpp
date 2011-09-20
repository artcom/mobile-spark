#include "Easing.h"

#include <math.h>
#define PI 3.14159265

#include <boost/bind.hpp>

namespace animation {

    FunctionPtr EasingFnc(EasingFunctionPtr4 theFncPtr) {
        boost::function<float(float,float,float,float)> myFncPtr = theFncPtr;
        return boost::bind(myFncPtr, _1,0,1,1);
    }
    FunctionPtr EasingFnc(EasingFunctionPtr5 theFncPtr, float a) {
        boost::function<float(float,float,float,float,float)> myFncPtr = theFncPtr;
        return boost::bind(myFncPtr, _1,0,1,1,a);
    }
    FunctionPtr EasingFnc(EasingFunctionPtr6 theFncPtr, float a, float b) {
        boost::function<float(float,float,float,float,float,float)> myFncPtr = theFncPtr;
        return boost::bind(myFncPtr, _1,0,1,1,a,b);
    }

/* This version of the Penner Easing Functions has been modified to use the namespace Easing. */

/*
  Easing Equations v1.5
  May 1, 2003
  (c) 2003 Robert Penner, all rights reserved.
  This work is subject to the terms in http://www.robertpenner.com/easing_terms_of_use.html.

  These tweening functions provide different flavors of
  math-based motion under a consistent API.

  Types of easing:

      Linear
      Quadratic
      Cubic
      Quartic
      Quintic
      Sinusoidal
      Exponential
      Circular
      Elastic
      Back
      Bounce

  Changes:
  1.5 - added bounce easing
  1.4 - added elastic and back easing
  1.3 - tweaked the exponential easing functions to make endpoints exact
  1.2 - inline optimizations (changing t and multiplying in one step)--thanks to Tatsuo Kato for the idea

  Discussed in Chapter 7 of
  Robert Penner's Programming Macromedia Flash MX
  (including graphs of the easing equations)

  http://www.robertpenner.com/profmx
  http://www.amazon.com/exec/obidos/ASIN/0072223561/robertpennerc-20
*/

// simple linear tweening - no easing
// t: current time, b: beginning value, c: change in value, d: duration
float linearTween(const float t, const float b, const float c, const float d) {
    return c*t/d + b;
}


 ///////////// QUADRATIC EASING: t^2 ///////////////////

// quadratic easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in value, d: duration
// t and d can be in frames or seconds/milliseconds
float easeInQuad(float t, const float b, const float c, const float d) {
    t/=d;
    return c*t*t + b;
}

// quadratic easing out - decelerating to zero velocity
float easeOutQuad(float t, const float b, const float c, const float d) {
    t/=d;
    return -c *t*(t-2) + b;
}

// quadratic easing in/out - acceleration until halfway, then deceleration
float easeInOutQuad(float t, const float b, const float c, const float d) {
    t/=d/2;
    if (t < 1) return c/2*t*t + b;
    --t;
    return -c/2 * (t*(t-2) - 1) + b;
}


 ///////////// CUBIC EASING: t^3 ///////////////////////

// cubic easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in value, d: duration
// t and d can be frames or seconds/milliseconds
float easeInCubic(float t, const float b, const float c, const float d) {
    t/=d;
    return c*t*t*t + b;
}

// cubic easing out - decelerating to zero velocity
float easeOutCubic(float t, const float b, const float c, const float d) {
    t=t/d-1;
    return c*t*t*t + 1 + b;
}

// cubic easing in/out - acceleration until halfway, then deceleration
float easeInOutCubic(float t, const float b, const float c, const float d) {
    t/=d/2;
    if (t < 1) return c/2*t*t*t + b;
    t-=2;
    return c/2*(t*t*t + 2) + b;
}


 ///////////// QUARTIC EASING: t^4 /////////////////////

// quartic easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in value, d: duration
// t and d can be frames or seconds/milliseconds
float easeInQuart(float t, const float b, const float c, const float d) {
    t/=d;
    return c*t*t*t*t + b;
}

// quartic easing out - decelerating to zero velocity
float easeOutQuart(float t, const float b, const float c, const float d) {
    t=t/d-1;
    return -c * (t*t*t*t - 1) + b;
}

// quartic easing in/out - acceleration until halfway, then deceleration
float easeInOutQuart(float t, const float b, const float c, const float d) {
    t/=d/2;
    if (t < 1) return c/2*t*t*t*t + b;
    t-=2;
    return -c/2 * (t*t*t*t - 2) + b;
}


 ///////////// QUINTIC EASING: t^5  ////////////////////

// quintic easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in value, d: duration
// t and d can be frames or seconds/milliseconds
float easeInQuint(float t, const float b, const float c, const float d) {
    t/=d;
    return c*t*t*t*t*t + b;
}

// quintic easing out - decelerating to zero velocity
float easeOutQuint(float t, const float b, const float c, const float d) {
    t=t/d-1;
    return c*(t*t*t*t*t + 1) + b;
}

// quintic easing in/out - acceleration until halfway, then deceleration
float easeInOutQuint(float t, const float b, const float c, const float d) {
    t/=d/2;
    if (t < 1) return c/2*t*t*t*t*t + b;
    t-=2;
    return c/2*(t*t*t*t*t + 2) + b;
}



 ///////////// SINUSOIDAL EASING: sin(t) ///////////////

// sinusoidal easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in position, d: duration
float easeInSine(const float t, const float b, const float c, const float d) {
    return -c * cos(t/d * (PI/2)) + c + b;
}

// sinusoidal easing out - decelerating to zero velocity
float easeOutSine(const float t, const float b, const float c, const float d) {
    return c * sin(t/d * (PI/2)) + b;
}

// sinusoidal easing in/out - accelerating until halfway, then decelerating
float easeInOutSine(const float t, const float b, const float c, const float d) {
    return -c/2 * (cos(PI*t/d) - 1) + b;
}


 ///////////// EXPONENTIAL EASING: 2^t /////////////////

// exponential easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in position, d: duration
float easeInExpo(const float t, const float b, const float c, const float d) {
    return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b;
}

// exponential easing out - decelerating to zero velocity
float easeOutExpo(const float t, const float b, const float c, const float d) {
    return (t==d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b;
}

// exponential easing in/out - accelerating until halfway, then decelerating
float easeInOutExpo(float t, const float b, const float c, const float d) {
    t/=d/2;
    if (t==0) return b;
    if (t==d) return b+c;
    if (t < 1) return c/2 * pow(2, 10 * (t - 1)) + b;
    return c/2 * (-pow(2, -10 * --t) + 2) + b;
}


 /////////// CIRCULAR EASING: sqrt(1-t^2) //////////////

// circular easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in position, d: duration
float easeInCirc(float t, const float b, const float c, const float d) {
    t/=d;
    return -c * (sqrt(1 - t*t) - 1) + b;
}

// circular easing out - decelerating to zero velocity
float easeOutCirc(float t, const float b, const float c, const float d) {
    t=t/d-1;
    return c * sqrt(1 - t*t) + b;
}

// circular easing in/out - acceleration until halfway, then deceleration
float easeInOutCirc(float t, const float b, const float c, const float d) {
    t/=d/2;
    if (t < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
    t-=2;
    return c/2 * (sqrt(1 - t*t) + 1) + b;
}


 /////////// ELASTIC EASING: exponentially decaying sine wave  //////////////

// t: current time, b: beginning value, c: change in value, d: duration, a: amplitude (optional), p: period (optional)
// t and d can be in frames or seconds/milliseconds
float easeInElastic(float t, const float b, const float c, const float d, float a, float p) {
    t/=d;
    if (t==0) return b;  if (t==1) return b+c;  if (p==-1) p=d*.3;
    float s = 0;
    if (a==-1 || a < fabs(c)) { a=c; s=p/4; }
    else s = p/(2*PI) * asin(c/a);
    t-=1;
    return -(a*pow(2,10*t) * sin( (t*d-s)*(2*PI)/p )) + b;
}

float easeOutElastic(float t, const float b, const float c, const float d, float a, float p) {
    t/=d;
    if (t==0) return b;  if (t==1) return b+c;  if (p==-1) p=d*.3;
    float s = 0;
    if (a==-1 || a < fabs(c)) { a=c; s=p/4; }
    else s = p/(2*PI) * asin (c/a);
    return (a*pow(2,-10*t) * sin( (t*d-s)*(2*PI)/p ) + c + b);
}

float easeInOutElastic(float t, const float b, const float c, const float d, float a, float p) {
    t/=d/2;
    if (t==0) return b;  if (t==2) return b+c;  if (p==-1) p=d*(.3*1.5);
    float s = 0;
    if (a==-1 || a < fabs(c)) { a=c; s=p/4; }
    else s = p/(2*PI) * asin (c/a);
    if (t < 1)  {
        t-=1;
        return -.5*(a*pow(2,10*t) * sin( (t*d-s)*(2*PI)/p )) + b;
    }
    t-=1;
    return a*pow(2,-10*t) * sin( (t*d-s)*(2*PI)/p )*.5 + c + b;
}


 /////////// BACK EASING: overshooting cubic easing: (s+1)*t^3 - s*t^2  //////////////

// back easing in - backtracking slightly, then reversing direction and moving to target
// t: current time, b: beginning value, c: change in value, d: duration, s: overshoot amount (optional)
// t and d can be in frames or seconds/milliseconds
// s controls the amount of overshoot: higher s means greater overshoot
// s has a default value of 1.70158, which produces an overshoot of 10 percent
// s==0 produces cubic easing with no overshoot
float easeInBack(float t, const float b, const float c, const float d, float s) {
    t/=d;
    if (s == -1) s = 1.70158;
    return c*t*t*((s+1)*t - s) + b;
}

// back easing out - moving towards target, overshooting it slightly, then reversing and coming back to target
float easeOutBack(float t, const float b, const float c, const float d, float s) {
    t=t/d-1;
    if (s == -1) s = 1.70158;
    return c*(t*t*((s+1)*t + s) + 1) + b;
}

// back easing in/out - backtracking slightly, then reversing direction and moving to target,
// then overshooting target, reversing, and finally coming back to target
float easeInOutBack(float t, const float b, const float c, const float d, float s) {
    t=d/2;
    if (s == -1) s = 1.70158;
    s*=1.525;
    if (t < 1) return c/2*(t*t*((s+1)*t - s)) + b;
    t-=2;
    return c/2*(t*t*((s+1)*t + s) + 2) + b;
}


 /////////// BOUNCE EASING: exponentially decaying parabolic bounce  //////////////

// bounce easing in
// t: current time, b: beginning value, c: change in position, d: duration
float easeInBounce(float t, const float b, const float c, const float d) {
    return c - easeOutBounce(d-t, 0, c, d) + b;
}

// bounce easing out
float easeOutBounce(float t, const float b, const float c, const float d) {
    t/=d;
    if (t < (1/2.75)) {
        return c*(7.5625*t*t) + b;
    } else if (t < (2/2.75)) {
        t-=1.5/2.75;
        return c*(7.5625*t*t + .75) + b;
    } else if (t < (2.5/2.75)) {
        t-=2.25/2.75;
        return c*(7.5625*t*t + .9375) + b;
    } else {
        t-=2.625/275;
        return c*(7.5625*t*t + .984375) + b;
    }
}

// bounce easing in/out
float easeInOutBounce(float t, const float b, const float c, const float d) {
    if (t < d/2) return easeInBounce(t*2, 0, c, d) * .5 + b;
    return easeOutBounce(t*2-d, 0, c, d) * .5 + c*.5 + b;
}



} //endof animation namespace

