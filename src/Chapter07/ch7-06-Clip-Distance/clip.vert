#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out VS_OUT {
  vec3 N;
  vec3 L;
  vec3 V;
}vs_out;

// Position of light
uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);

// Clip plane
uniform vec4 clip_plane = vec4(1.0, 1.0, 0.0, 0.85); 
uniform vec4 clip_sphere = vec4(0.0, 0.0, 0.0, 4.0);

void main()
{
   vec4 P = mv_matrix * position ;
   vs_out.N = mat3(mv_matrix) * normal; //normal
   vs_out.L = light_pos - P.xyz;        //light
   vs_out.V = -P.xyz;                   //view

    // Write clip distance, > 0 pass
    gl_ClipDistance[0] = dot(position, clip_plane);  
    gl_ClipDistance[1] = length(position.xyz / position.w - clip_sphere.xyz) - clip_sphere.w;

    // Calculate the clip-space position of each vertex
    gl_Position = proj_matrix * P;
}


