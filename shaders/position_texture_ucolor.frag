#ifdef GL_ES
precision lowp float;
#endif

uniform vec4 u_Color;
uniform sampler2D u_Sampler;

varying vec2 v_TexCoord;

void main() {
    gl_FragColor = u_Color * texture2D(u_Sampler, v_TexCoord);
}
