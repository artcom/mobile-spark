precision mediump float;
precision lowp int;
 
uniform mat4 u_mvpMatrix;
uniform mat4 u_textureMatrix;
attribute vec4 a_position;
attribute vec4 a_texCoord0;
varying vec4 v_texCoord;
uniform float       a_time;
uniform float       a_mode;
 
void main() {
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
    
    v_texCoord = a_texCoord0 * u_textureMatrix;
    v_texCoord.s = startS + v_texCoord.s * (startScale + (endScale - startScale) * a_time) + moveS * a_time;
    v_texCoord.t = startT + v_texCoord.t * (startScale + (endScale - startScale) * a_time) + moveT * a_time;

 
    // Here we set the final position to this vertex.
    gl_Position = u_mvpMatrix * a_position;
}


