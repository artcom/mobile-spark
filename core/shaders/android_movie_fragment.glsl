// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#extension GL_OES_EGL_image_external : require
precision mediump float;
precision lowp int;
 
uniform samplerExternalOES s_textureMap;
varying vec4               v_texCoord;
uniform float              u_alpha;
 
void main() {
    vec4 tex = texture2D(s_textureMap, v_texCoord.xy);
    gl_FragColor = vec4(tex.rgb * u_alpha, tex.a * u_alpha);
}



