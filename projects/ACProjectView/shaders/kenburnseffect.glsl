precision mediump float;
precision lowp int;
 
uniform sampler2D   s_textureMap;
varying vec2        v_texCoord;
uniform float       a_alpha; 
uniform float       a_time;
uniform float       a_mode;
 
void main() {
    vec2 texCoord = v_texCoord;

    float startS, startT, moveS, moveT, startScale, endScale;
    if (a_mode < 0.2) {
        startS = 0.5;
        startT = 0.1;
        moveS = -0.2;
        moveT = 0.3;
        startScale = 0.2;
        endScale = 0.5;
    } else if (a_mode < 0.4) {
        startS = 0.1;
        startT = 0.2;
        moveS = 0.3;
        moveT = 0.1;
        startScale = 0.2;
        endScale = 0.5;
    } else if (a_mode < 0.6) {
        startS = 0.0;
        startT = 0.3;
        moveS = 0.2;
        moveT = -0.2;
        startScale = 0.2;
        endScale = 0.5;
    } else if (a_mode < 0.8) {
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
    
    texCoord.s = startS + texCoord.s * (startScale + (endScale - startScale) * a_time) + moveS * a_time;
    texCoord.t = startT + texCoord.t * (startScale + (endScale - startScale) * a_time) + moveT * a_time;

    vec4 tex = texture2D(s_textureMap, texCoord);
    gl_FragColor = vec4(tex.rgb, tex.a * a_alpha);
}


