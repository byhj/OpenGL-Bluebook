#version 430 core

layout (location = 0) out vec4 fragColor;

in vec4 vColor;

void main(void)
{
   fragColor = vColor;
}