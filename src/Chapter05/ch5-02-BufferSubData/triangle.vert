#version 430 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Color;

out VS_OUT 
{
   vec4 color;
}vs_out;

void main(void)
{
   vs_out.color = Color;
   gl_Position = Position;
}