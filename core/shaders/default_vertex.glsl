precision mediump float;
precision lowp int;
 
uniform mat4 u_mvpMatrix;
attribute vec4 a_position;
 
void main() {
	 
    // Here we set the final position to this vertex.
    gl_Position = u_mvpMatrix * a_position;

}

