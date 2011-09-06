precision mediump float;
precision lowp int;

uniform vec4 a_color; 
 
void main() {
    gl_FragColor = a_color;
    //gl_FragColor = vec4(0.0,1.0,1.0,1.0);
}

