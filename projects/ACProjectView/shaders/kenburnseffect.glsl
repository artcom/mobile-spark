precision mediump float;
precision lowp int;
 
uniform sampler2D   s_textureMap;
varying vec2        v_texCoord;
uniform float       a_alpha; 
uniform float       a_time;
 
void main() {
    vec2 texCoord = v_texCoord;
    float startS = 0.5;
    float startT = 0.1;
    float moveS = -0.2;
    float moveT = 0.3;
    float startScale = 0.2;
    float endScale = 0.5;
    
    texCoord.s = startS + texCoord.s * (startScale + (endScale - startScale) * a_time) + moveS * a_time;
    texCoord.t = startT + texCoord.t * (startScale + (endScale - startScale) * a_time) + moveT * a_time;
    vec4 tex = texture2D(s_textureMap, texCoord);
    gl_FragColor = vec4(tex.rgb, tex.a * a_alpha);
}


