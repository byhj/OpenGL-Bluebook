#version 410 core

// Per-vertex inputs
layout (location = 0) in vec4 position; 
layout (location = 1) in vec3 normal;

uniform mat4 mv_matrix;
uniform mat4 mvp_matrix;

// Inputs from vertex shader
out VS_OUT
{
    vec3 normal;
    vec4 color;
} vs_out;

void main(void)
{
   gl_Position = mvp_matrix * position; 
   vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0); 
   vs_out.normal = normalize(mat3(mv_matrix) * normal);
}
