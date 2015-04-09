#version 430 core

const vec3 Position[3] = vec3[3](vec3(0.25f, -0.25f, 0.5f),
	                             vec3(-0.25f, -0.25f, 0.5f),
	                              vec3(0.25f, 0.25f, 0.5f)
                                );

void main()
{
   gl_Position = vec4(Position[gl_VertexID], 1.0f); //gl_VertexID当前顶点索引
}