#version 450 core

const vec2 positions[4] = vec2[4](
        vec2(-0.5, -0.5),
        vec2(0.5, -0.5),
        vec2(-0.5, 0.5),
        vec2(0.5, 0.5)
    );

void main() {
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
