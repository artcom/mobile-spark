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
attribute vec4 a_position;
 
void main() {
	 
    // Here we set the final position to this vertex.
    gl_Position = u_mvpMatrix * a_position;

}

