#version 430 core

layout (location = 0) in vec4 Position;

out vec4 vColor;

uniform mat4 mvp;

void main(void)
{
 
   vColor = Position  + vec4(0.5f); 
   gl_Position = mvp * Position;
}