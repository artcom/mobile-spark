precision mediump float;
precision lowp int;
 
uniform mat4 u_mvpMatrix;
attribute vec4 a_position;
attribute vec2 a_texCoord0;
varying vec2 v_texCoord;
 
void main() {
    // Pass the texture coordinate attribute to a varying.
    //v_texCoord = a_texCoord0;
 
    // Here we set the final position to this vertex.
    gl_Position = u_mvpMatrix * a_position;
    //gl_Position = a_position;
}

