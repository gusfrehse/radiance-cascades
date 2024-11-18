#version 450 core

out vec4 frag_color;
in vec2 v_uv;

uniform sampler2D s_input;
uniform int u_step;

void main() {
    float best_dist = 99999999.9;
    vec2 best_coord = vec2(0.0);
    float offset = exp2(float(-u_step)); // this may be wrong

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 pos = v_uv + vec2(x * offset, y * offset);
            vec2 data = texture(s_input, pos).xy;

            if (data == vec2(0.)) {
                continue;
            }

            float dist = distance(v_uv, data);
            if (dist < best_dist) {
                best_dist = dist;
                best_coord = pos;
            }
        }
    }

    frag_color = vec4(best_coord, best_dist, 1.0);
}
