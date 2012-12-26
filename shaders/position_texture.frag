#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D u_Sampler;

varying vec2 v_TexCoord;
varying vec4 v_FragmentColor;

void main() {
    gl_FragColor = texture2D(u_Sampler, v_TexCoord);
}
