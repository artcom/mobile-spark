precision mediump float;
precision lowp int;
 
uniform sampler2D   s_textureMap;
varying vec4        v_texCoord;
uniform float u_alpha; 
 
void main() {
    vec4 tex = texture2D(s_textureMap, v_texCoord.xy);
    gl_FragColor = vec4(tex.rgb, tex.a * u_alpha);
}


