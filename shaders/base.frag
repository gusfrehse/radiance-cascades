#version 450 core

out vec4 frag_color;
in vec2 v_uv;

uniform sampler2D s_input;

void main() {
    frag_color = texture(s_input, v_uv);
    // frag_color = vec4(gl_FragCoord.xyz, 1.0);
}
