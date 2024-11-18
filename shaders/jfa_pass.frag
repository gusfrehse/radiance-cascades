#version 450 core

out vec4 frag_color;
in vec2 v_uv;

uniform sampler2D s_input;
uniform int u_step;

void main() {
    float best_dist = 99999999.9;
    vec2 best_seed = vec2(0.0);
    float offset = exp2(float(-u_step / 2.0)); // this may be wrong

    bool exit = false;
    float found = 0.0;

    for (int x = -1; x <= 1 && !exit; x++) {
        for (int y = -1; y <= 1 && !exit; y++) {
            vec2 pos = 0.5 + v_uv + vec2(x * offset, y * offset);
            vec4 data = texture(s_input, pos);

            if (data.w == 0.0) {
                continue;
            }

            vec2 closest_seed = data.xy;

            float dist = distance(v_uv, closest_seed);
            if (dist < best_dist) {
                best_dist = dist;
                best_seed = closest_seed;
                found = 1.0;
            }
        }
    }

    frag_color = vec4(best_seed, 0.0, found);
}
