precision mediump float;
precision lowp int;
 
uniform mat4 u_mvpMatrix;
uniform mat4 u_textureMatrix;
attribute vec4 a_position;
attribute vec4 a_texCoord0;
varying vec4 v_texCoord;
 
void main() {
    // Pass the texture coordinate attribute to a varying.
    v_texCoord = a_texCoord0 * u_textureMatrix;
 
    // Here we set the final position to this vertex.
    gl_Position = u_mvpMatrix * a_position;

}

