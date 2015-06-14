#version 430 core

layout (location = 0) in vec4 offset;

const vec4 Position[3] = 
{
   vec4(-0.5f, -0.5f, 0.0f, 1.0f),
   vec4( 0.5f, -0.5f, 0.0f, 1.0f),
   vec4( 0.0f,  0.5f, 0.0f, 1.0f)
};

const vec4 Color[3] = 
{
   vec4(1.0f,  0.0f, 0.0f, 1.0f),
   vec4(0.0f,  1.0f, 0.0f, 1.0f),
   vec4(0.0f,  0.0f, 1.0f, 1.0f)
};

//Use flat or smooth
out VS_OUT 
{
 flat vec4 flatColor;
 smooth vec4 smoothColor;
}vs_out;

void main(void)
{
   //gl_VertexID is current vertex index

   vs_out.flatColor = Color[gl_VertexID];
   vs_out.smoothColor = Color[gl_VertexID];
   gl_Position = Position[gl_VertexID] + offset;
}