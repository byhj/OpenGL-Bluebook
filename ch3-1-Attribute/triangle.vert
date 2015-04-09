#version 430

layout (location = 0) in vec3 offset;

const vec3 Position[3] = vec3[3](vec3(0.25f, -0.25f, 0.5f),
	                      vec3(-0.25f, -0.25f, 0.5f),
	                       vec3(0.25f, 0.25f, 0.5f)
                          );

void main()
{
   gl_Position = vec4(Position[gl_VertexID] + offset, 1.0f); //gl_VertexID��ǰ��������
}