precision mediump float;
precision lowp int;
 
uniform sampler2D   s_textureMap;
varying vec2        v_texCoord;
 
void main() {
    gl_FragColor = texture2D(s_textureMap, v_texCoord);
}


