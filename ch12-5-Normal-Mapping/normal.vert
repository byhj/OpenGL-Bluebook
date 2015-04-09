#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
// Although the model file used in this example includes
// a bitangent, we're not using it and will calculate our
// own from the normal and tangent.
// layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texcoord;


uniform mat4 mv_matrix;
uniform mat4 mvp_matrix;

out VS_OUT
{
    vec2 texcoord;
    vec3 eyeDir;
    vec3 lightDir;
    vec3 normal;
} vs_out;

uniform vec3 lightPos = vec3(0.0, 0.0, 100.0);

void main(void)
{
    vec4 P = mv_matrix * position;
    vec3 V = P.xyz;
    vec3 N = normalize(mat3(mv_matrix) * normal);
    vec3 T = normalize(mat3(mv_matrix) * tangent);

	 // Calculate the bitangent vector (B) from the normal and tangent
	vec3 B = cross(N, T);
	vec3 L = lightPos - P.xyz;

	//calc the TBN
	vs_out.lightDir = normalize(vec3(dot(L, T), dot(L, B), dot(L, N)));
    vs_out.eyeDir = normalize(vec3(dot(-V, T), dot(-V, B), dot(-V, N)));

    vs_out.texcoord = texcoord;
    vs_out.normal = N;

   gl_Position = mvp_matrix * position;
}
