// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

precision mediump float;
precision lowp int;
 
uniform mat4 u_mvpMatrix;
uniform mat4 u_textureMatrix;
attribute vec4 a_position;
attribute vec4 a_texCoord0;
varying vec4 v_texCoord;
uniform float       u_time;
uniform float       u_mode;
 
void main() {
    float startS, startT, moveS, moveT, startScale, endScale;
    if (u_mode < 0.2) {
        startS = 0.5;
        startT = 0.1;
        moveS = -0.2;
        moveT = 0.3;
        startScale = 0.2;
        endScale = 0.5;
    } else if (u_mode < 0.4) {
        startS = 0.1;
        startT = 0.2;
        moveS = 0.3;
        moveT = 0.1;
        startScale = 0.2;
        endScale = 0.5;
    } else if (u_mode < 0.6) {
        startS = 0.0;
        startT = 0.3;
        moveS = 0.2;
        moveT = -0.2;
        startScale = 0.2;
        endScale = 0.5;
    } else if (u_mode < 0.8) {
        startS = 0.3;
        startT = 0.3;
        moveS = -0.2;
        moveT = 0.3;
        startScale = 0.5;
        endScale = 0.2;
    } else {
        startS = 0.4;
        startT = 0.2;
        moveS = -0.2;
        moveT = -0.2;
        startScale = 0.4;
        endScale = 0.6;
    }
    
    v_texCoord = a_texCoord0 * u_textureMatrix;
    v_texCoord.s = startS + v_texCoord.s * (startScale + (endScale - startScale) * u_time) + moveS * u_time;
    v_texCoord.t = startT + v_texCoord.t * (startScale + (endScale - startScale) * u_time) + moveT * u_time;

 
    // Here we set the final position to this vertex.
    gl_Position = u_mvpMatrix * a_position;
}


