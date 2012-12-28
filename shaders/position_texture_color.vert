uniform mat4 u_MVPMatrix;

attribute vec4 a_Position;
attribute vec2 a_TexCoord;
attribute vec4 a_Color;

#ifdef GL_ES
varying lowp vec4 v_FragmentColor;
varying mediump vec2 v_TexCoord;
#else
varying vec4 v_FragmentColor;
varying vec2 v_TexCoord;
#endif

void main() {
    gl_Position = u_MVPMatrix * a_Position;
    v_FragmentColor = a_Color;
    v_TexCoord = a_TexCoord;
}

