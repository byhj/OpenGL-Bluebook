#version 430 core

layout (location = 0) in vec4 Position;
layout (location = 4) in vec2 TexCoord;

uniform mat4 mvp;

out vec2 tc;

void main(void)
{
   tc = TexCoord;
   gl_Position = mvp * Position;
}