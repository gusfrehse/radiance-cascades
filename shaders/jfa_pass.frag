#version 450 core

out vec4 frag_color;
in vec2 v_uv;

uniform sampler2D s_input;
uniform int u_step;
//uniform vec2 u_pixel_size;

void main() {
    float best_dist = 99999999.9;
    vec2 best_seed = vec2(0.0);
    float offset = int(exp2(float(10.0 - u_step))); // this may be wrong

    bool exit = false;
    float found = 0.0;

    vec2 u_pixel_size = vec2(1.0 / 854.0, 1.0 / 480.0);

    for (int x_sign = -1; x_sign <= 1 && !exit; x_sign++) {
        for (int y_sign = -1; y_sign <= 1 && !exit; y_sign++) {
            vec2 pos = v_uv + u_pixel_size * vec2(x_sign * offset, y_sign * offset);
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

    //frag_color = vec4(best_seed, 0.0, found);
    frag_color = vec4(0.0, 0.0, best_dist, found);
    //frag_color = vec4(v_uv, 0.0, found);
}
