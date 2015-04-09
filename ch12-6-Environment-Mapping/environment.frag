#version 410 core

uniform sampler2D tex_envmap;

in VS_OUT
{
    vec3 normal;
    vec3 view;
} fs_in;

out vec4 color;

void main(void)
{
    vec3 view = normalize(fs_in.view);
    vec3 r = reflect(view, normalize(fs_in.normal));

    // Compute scale factor
    r.z += 1.0;
    float m = 0.5 * inversesqrt(dot(r, r));

    // Sample from scaled and biased texture coordinate
    color = texture(tex_envmap, r.xy * m + vec2(0.5));
}
