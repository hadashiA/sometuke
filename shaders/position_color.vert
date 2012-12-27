uniform mat4 u_MVPMatrix;

attribute vec4 a_Position;
attribute vec4 a_Color;

#ifdef GL_ES
varying lowp vec4 v_FragmentColor;
#else
varying vec4 v_FragmentColor;
#endif

void main() {
    gl_Position = u_MVPMatrix * a_Position;
    v_FragmentColor = a_Color;
}
