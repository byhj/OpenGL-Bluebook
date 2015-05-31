#version 430 core

layout (location = 0) in vec4 offset;

const vec4 Position[3] = 
{
   vec4(-0.5f, -0.5f, 0.0f, 1.0f),
   vec4( 0.5f, -0.5f, 0.0f, 1.0f),
   vec4( 0.0f,  0.5f, 0.0f, 1.0f)
};


void main(void)
{
   gl_Position = Position[gl_VertexID] + offset;
}