#version 430 core

layout (location = 0) in vec4 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
  gl_Position = proj_matrix * mv_matrix * position;
}