precision mediump float;
precision lowp int;
 
uniform sampler2D   s_textureMap;
varying vec2        v_texCoord;
uniform float a_alpha; 
 
void main() {
    vec4 tex = texture2D(s_textureMap, v_texCoord);
    //gl_FragColor = vec4(tex.rgb, tex.a * a_alpha);
    gl_FragColor = vec4(1.0,0.0,0.0,1.0 * a_alpha);
}


