precision mediump float;
precision lowp int;

uniform vec4 u_color; 
uniform float u_alpha; 
 
void main() {
    gl_FragColor = vec4(u_color.rgb, u_color.a * u_alpha);
}

