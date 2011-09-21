precision mediump float;
precision lowp int;

uniform vec4 a_color; 
uniform float a_alpha; 
 
void main() {
    gl_FragColor = vec4(a_color.rgb, a_color.a * a_alpha);
}

