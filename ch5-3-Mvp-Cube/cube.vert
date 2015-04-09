#version 430 core

layout (location = 0) in vec4 Position;

out VS_OUT {
  vec4 color;
}vs_out;

uniform mat4 mvp_matrix;

void main()
{

   gl_Position = mvp_matrix * Position;
   vs_out.color = Position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}