#version 430 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 color;

out VS_OUT {
  vec4 color;
}vs_out;

void main()
{

   gl_Position = Position;
   vs_out.color = color;
}