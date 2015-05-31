#version 430 core

out vec2 tc;

const vec4 Position[4] = 
{
   vec4( 0.5f, -0.5f, 0.0f, 1.0f),
   vec4( 0.5f,  0.5f, 0.0f, 1.0f),
   vec4(-0.5f, -0.5f, 0.0f, 1.0f),
   vec4(-0.5f,  0.5f, 0.0f, 1.0f)
};


void main(void)
{
   tc = Position[gl_VertexID].xy + vec2(0.5);
   gl_Position = Position[gl_VertexID];
}