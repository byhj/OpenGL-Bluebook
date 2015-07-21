#ifndef COMMON_H
#define COMMON_H

#include <gl/glew.h>
#include <glm/glm.hpp>

#define BUFFER_OFFSET(offset) ((GLvoid*)(NULL + offset))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

void videoCardInfo()
{
	const GLubyte *renderer = glGetString( GL_RENDERER );  
	const GLubyte *vendor = glGetString( GL_VENDOR );  
	const GLubyte *version = glGetString( GL_VERSION );  
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );  
	GLint major, minor;  
	glGetIntegerv(GL_MAJOR_VERSION, &major);  
	glGetIntegerv(GL_MINOR_VERSION, &minor);  
	std::cout << "GL Vendor    : " << vendor << std::endl;  
	std::cout << "GL Renderer  : " << renderer << std::endl;  
	std::cout << "GL Version   : " << version << std::endl;  
	std::cout << "GL Version   : " << major << "." << minor << std::endl;  
	std::cout << "GLSL Version : " << glslVersion << std::endl;    
	std::cout << "------------------------------------------------------------------------------" << std::endl;
}

namespace Color
{
	GLfloat black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
}

struct Vertex
{
	glm::vec3 pos; //顶点，纹理，法线坐标
	glm::vec2 tex;
	glm::vec3 normal;
	Vertex(glm::vec3 mpos, glm::vec2 mtex)
	{
		pos = mpos;
		tex = mtex;
		normal = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	Vertex(glm::vec3 mpos, glm::vec2 mtex, glm::vec3 n)
	{
		pos = mpos;
		tex = mtex;
		normal = n;
	}
};

//计算法线坐标
void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
				 Vertex* pVertices, unsigned int VertexCount)
{
	// Accumulate each triangle normal into each of the triangle vertices
	for (unsigned int i = 0; i < IndexCount; i += 3) 
	{ 
		unsigned int Index0 = pIndices[i];  
		unsigned int Index1 = pIndices[i + 1];
		unsigned int Index2 = pIndices[i + 2]; 
		glm::vec3 v1 = pVertices[Index1].pos - pVertices[Index0].pos;
		glm::vec3 v2 = pVertices[Index2].pos - pVertices[Index0].pos;
		glm::vec3 Normal = glm::normalize(glm::cross(v1, v2));

		pVertices[Index0].normal += Normal;
		pVertices[Index1].normal += Normal;
		pVertices[Index2].normal += Normal;
	}
	// Normalize all the vertex normals
	for (unsigned int i = 0; i < VertexCount; i++)
	{ 
		pVertices[i].normal = glm::normalize(pVertices[i].normal);
	}
}



#endif // !COMMON_H
