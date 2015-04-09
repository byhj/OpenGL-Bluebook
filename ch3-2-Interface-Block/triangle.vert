#version 430 core

layout (location = 0) in vec3 offset;
layout (location = 1) in vec4 color;

out VS_OUT {
   vec4 color;
} vs_out;

const vec3 Position[3] = vec3[3](vec3(0.25f, -0.25f, 0.5f),
	                      vec3(-0.25f, -0.25f, 0.5f),
	                       vec3(0.25f, 0.25f, 0.5f)
                          );

void main()
{
   gl_Position = vec4(Position[gl_VertexID] + offset, 1.0f); //gl_VertexID当前顶点索引
   vs_out.color = color;
}