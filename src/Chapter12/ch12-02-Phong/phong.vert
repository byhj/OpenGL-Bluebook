#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 mv_matrix;
uniform mat4 mvp_matrix;

out VS_OUT 
{
  vec3 N;
  vec3 L;
  vec3 V;
}vs_out;

uniform vec3 lightPos = vec3(100.0, 100.0, 100.0);

void main(void)
{
   vec4 P = mv_matrix * position;
   vec3 N = mat3(mv_matrix) * normal;
   vec3 L = lightPos - P.xyz;
   vec3 V = - P.xyz;
   vs_out.N = N;
   vs_out.L = L;
   vs_out.V = V;
   gl_Position = mvp_matrix * position;
}
