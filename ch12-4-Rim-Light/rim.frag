#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT {
   vec3 N;
   vec3 L;
   vec3 V;
}fs_in;


uniform vec3 diffuse_albedo = vec3(0.5, 0.2, 0.7);
uniform vec3 specular_albedo = vec3(0.7);
uniform float specular_power = 128.0;
uniform vec3 ambient = vec3(0.1);
uniform vec3 rim_color = vec3(0.1, 0.7, 0.2);
uniform float rim_power = 5.0;

vec3 calculate_rim(vec3 N, vec3 V) // cal the rim light
{
    float f = 1.0 - dot(N, V);

    f = smoothstep(0.0, 1.0, f);
    f = pow(f, rim_power);

    return f * rim_color;
}


void main(void)
{
   vec3 N = normalize(fs_in.N);
   vec3 L = normalize(fs_in.L);
   vec3 V = normalize(fs_in.V);

   vec3 R = reflect(-L, N);
   vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
   vec3 specular = pow( max(dot(R, V), 0.0), specular_power ) * specular_albedo;
   vec3 rim = calculate_rim(N, V);

   fragColor = vec4(diffuse + specular + rim, 1.0);
}