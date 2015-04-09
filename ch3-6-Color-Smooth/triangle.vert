#version 430 core

const vec3 Position[3] = vec3[3](vec3(0.25f, -0.25f, 0.5f),
	                             vec3(-0.25f, -0.25f, 0.5f),
	                             vec3(0.25f, 0.25f, 0.5f)
                              );
const vec4 color[3] = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),
                               vec4(0.0, 1.0, 0.0, 1.0),
							   vec4(0.0, 0.0, 1.0, 1.0)
							   );
out VS_OUT {
   vec4 color;
 }vs_out;

void main()
{

   gl_Position = vec4(Position[gl_VertexID], 1.0f); //gl_VertexID当前顶点索引
   vs_out.color = color[gl_VertexID];

}