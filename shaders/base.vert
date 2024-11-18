#version 450 core

const vec2 positions[4] = vec2[4](
        vec2(-0.5, -0.5),
        vec2(0.5, -0.5),
        vec2(-0.5, 0.5),
        vec2(0.5, 0.5)
    );
    
const vec2 uvs[4] = vec2[4](
        vec2(0.0, 0.0),
        vec2(1.0, 0.0),
        vec2(0.0, 1.0),
        vec2(1.0, 1.0)
    );

out vec2 v_uv;

void main() {
    gl_Position = vec4(2.0 * positions[gl_VertexID], 0.0, 1.0);
    v_uv = uvs[gl_VertexID];
}
