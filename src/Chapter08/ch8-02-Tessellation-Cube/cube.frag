#version 430 core

in vec3 normal;
out vec4 fcolor;

void main(void) 
{
        fcolor = vec4(abs(normal), 1.0); 
}