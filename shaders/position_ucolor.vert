uniform mat4 u_MVPMatrix;
uniform	vec4 u_Color;
uniform float u_PointSize;

attribute vec4 a_Position;

#ifdef GL_ES
varying lowp vec4 v_FragmentColor;
#else
varying vec4 v_FragmentColor;
#endif

void main() {
    // gl_Position = u_MVPMatrix * a_Position; 
    gl_Position = vec4(0, 0, 0, 1);
    
    gl_PointSize = u_PointSize;
    v_FragmentColor = u_Color;
}
