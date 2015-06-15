#version 430 core

out vec4 fragColor;

// Color and normal maps
layout (binding = 0) uniform sampler2D tex_color;
layout (binding = 1) uniform sampler2D tex_normal;

in VS_OUT
{
    vec2 texcoord;
    vec3 eyeDir;
    vec3 lightDir;
    vec3 normal;
} fs_in;

void main(void)
{
    vec3 V = normalize(fs_in.eyeDir);
    vec3 L = normalize(fs_in.lightDir);

    // Read the normal from the normal map and normalize it.
    vec3 N = normalize(texture(tex_normal, fs_in.texcoord).rgb * 2.0 - vec3(1.0));
    vec3 R = reflect(-L, N);

    vec3 diffuse_albedo = texture(tex_color, fs_in.texcoord).rgb;
    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
    vec3 specular_albedo = vec3(1.0);
    vec3 specular = max(pow(dot(R, V), 20.0), 0.0) * specular_albedo;

	fragColor = vec4(diffuse + specular, 1.0);
}