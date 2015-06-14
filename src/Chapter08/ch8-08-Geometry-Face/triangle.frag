#version 430 core

flat in vec4 color;
out vec4 fragColor;

void main(void) 
{
    fragColor = color; 
}