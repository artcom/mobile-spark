// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_animation_Easing_h_included_
#define _ac_mobile_animation_Easing_h_included_

#include <boost/function.hpp>

namespace animation {

    //function conversion
    typedef boost::function<float(float)> FunctionPtr;
    typedef float (*EasingFunctionPtr4)(float, float, float, float);
    typedef float (*EasingFunctionPtr5)(float, float, float, float, float);
    typedef float (*EasingFunctionPtr6)(float, float, float, float, float, float);

    FunctionPtr EasingFnc(EasingFunctionPtr4 theFncPtr);
    FunctionPtr EasingFnc(EasingFunctionPtr5 theFncPtr, float a = -1.0);
    FunctionPtr EasingFnc(EasingFunctionPtr6 theFncPtr, float a = -1.0, float b = -1.0);


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
float linearTween(const float t, const float b, const float c, const float d);


 ///////////// QUADRATIC EASING: t^2 ///////////////////

// quadratic easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in value, d: duration
// t and d can be in frames or seconds/milliseconds
float easeInQuad(float t, const float b, const float c, const float d);

// quadratic easing out - decelerating to zero velocity
float easeOutQuad(float t, const float b, const float c, const float d);

// quadratic easing in/out - acceleration until halfway, then deceleration
float easeInOutQuad(float t, const float b, const float c, const float d);


 ///////////// CUBIC EASING: t^3 ///////////////////////

// cubic easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in value, d: duration
// t and d can be frames or seconds/milliseconds
float easeInCubic(float t, const float b, const float c, const float d);

// cubic easing out - decelerating to zero velocity
float easeOutCubic(float t, const float b, const float c, const float d);

// cubic easing in/out - acceleration until halfway, then deceleration
float easeInOutCubic(float t, const float b, const float c, const float d);


 ///////////// QUARTIC EASING: t^4 /////////////////////

// quartic easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in value, d: duration
// t and d can be frames or seconds/milliseconds
float easeInQuart(float t, const float b, const float c, const float d);

// quartic easing out - decelerating to zero velocity
float easeOutQuart(float t, const float b, const float c, const float d);

// quartic easing in/out - acceleration until halfway, then deceleration
float easeInOutQuart(float t, const float b, const float c, const float d);


 ///////////// QUINTIC EASING: t^5  ////////////////////

// quintic easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in value, d: duration
// t and d can be frames or seconds/milliseconds
float easeInQuint(float t, const float b, const float c, const float d);

// quintic easing out - decelerating to zero velocity
float easeOutQuint(float t, const float b, const float c, const float d);

// quintic easing in/out - acceleration until halfway, then deceleration
float easeInOutQuint(float t, const float b, const float c, const float d);



 ///////////// SINUSOIDAL EASING: sin(t) ///////////////

// sinusoidal easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in position, d: duration
float easeInSine(const float t, const float b, const float c, const float d);

// sinusoidal easing out - decelerating to zero velocity
float easeOutSine(const float t, const float b, const float c, const float d);

// sinusoidal easing in/out - accelerating until halfway, then decelerating
float easeInOutSine(const float t, const float b, const float c, const float d);


 ///////////// EXPONENTIAL EASING: 2^t /////////////////

// exponential easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in position, d: duration
float easeInExpo(const float t, const float b, const float c, const float d);

// exponential easing out - decelerating to zero velocity
float easeOutExpo(const float t, const float b, const float c, const float d);

// exponential easing in/out - accelerating until halfway, then decelerating
float easeInOutExpo(float t, const float b, const float c, const float d);


 /////////// CIRCULAR EASING: sqrt(1-t^2) //////////////

// circular easing in - accelerating from zero velocity
// t: current time, b: beginning value, c: change in position, d: duration
float easeInCirc(float t, const float b, const float c, const float d);

// circular easing out - decelerating to zero velocity
float easeOutCirc(float t, const float b, const float c, const float d);

// circular easing in/out - acceleration until halfway, then deceleration
float easeInOutCirc(float t, const float b, const float c, const float d);


 /////////// ELASTIC EASING: exponentially decaying sine wave  //////////////

// t: current time, b: beginning value, c: change in value, d: duration, a: amplitude (optional), p: period (optional)
// t and d can be in frames or seconds/milliseconds

float easeInElastic(float t, const float b, const float c, const float d, float a = -1, float p = -1);

float easeOutElastic(float t, const float b, const float c, const float d, float a = -1, float p = -1);

float easeInOutElastic(float t, const float b, const float c, const float d, float a = -1, float p = -1);


 /////////// BACK EASING: overshooting cubic easing: (s+1)*t^3 - s*t^2  //////////////

// back easing in - backtracking slightly, then reversing direction and moving to target
// t: current time, b: beginning value, c: change in value, d: duration, s: overshoot amount (optional)
// t and d can be in frames or seconds/milliseconds
// s controls the amount of overshoot: higher s means greater overshoot
// s has a default value of 1.70158, which produces an overshoot of 10 percent
// s==0 produces cubic easing with no overshoot
float easeInBack(float t, const float b, const float c, const float d, float s = -1);

// back easing out - moving towards target, overshooting it slightly, then reversing and coming back to target
float easeOutBack(float t, const float b, const float c, const float d, float s = -1);

// back easing in/out - backtracking slightly, then reversing direction and moving to target,
// then overshooting target, reversing, and finally coming back to target
float easeInOutBack(float t, const float b, const float c, const float d, float s = -1);


 /////////// BOUNCE EASING: exponentially decaying parabolic bounce  //////////////

// bounce easing in
// t: current time, b: beginning value, c: change in position, d: duration
float easeInBounce(float t, const float b, const float c, const float d);

// bounce easing out
float easeOutBounce(float t, const float b, const float c, const float d);

// bounce easing in/out
float easeInOutBounce(float t, const float b, const float c, const float d);


};  //endof namespace animation

#endif

