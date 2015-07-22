#version 430 core

layout (location = 0) in vec4 Position;

const vec4 Color[3] = 
{
   vec4(1.0f,  0.0f, 0.0f, 1.0f),
   vec4(0.0f,  1.0f, 0.0f, 1.0f),
   vec4(0.0f,  0.0f, 1.0f, 1.0f)
};

out VS_OUT 
{
   vec4 color;
}vs_out;

void main(void)
{
   vs_out.color = Color[gl_VertexID];
   gl_Position = Position;
}