#version 430 core

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

//We pas the color data to fragment shader
out vec4 vColor;

void main(void)
{
   //gl_VertexID is current vertex index

   vColor = Color[gl_VertexID];
   gl_Position = Position[gl_VertexID];
}