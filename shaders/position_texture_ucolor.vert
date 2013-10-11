uniform mat4 u_MVPMatrix;

attribute vec4 a_Position;
attribute vec2 a_TexCoord;
attribute vec4 a_Color;

#ifdef GL_ES
varying mediump vec2 v_TexCoord;
#else
varying vec2 v_TexCoord;
#endif

void main() {
    vec4 color = a_Color;
    gl_Position = u_MVPMatrix * a_Position;
    v_TexCoord = a_TexCoord;
}

