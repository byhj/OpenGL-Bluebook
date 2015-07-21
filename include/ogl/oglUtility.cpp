#include "oglUtility.h"

//计算法线坐标
void CalcNormals(const GLuint *pIndexData, GLsizei IndexCount,
				 byhj::Vertex *pVertexData, GLsizei VertexCount)
{
	// Accumulate each triangle normal into each of the triangle vertices
	for (GLsizei i = 0; i < IndexCount; i += 3) 
	{ 
		//Get the three vertex index of triangle
		unsigned int Index0 = pIndexData[i];  
		unsigned int Index1 = pIndexData[i + 1];
		unsigned int Index2 = pIndexData[i + 2]; 

		//Calc the normal vector
		glm::vec3 v1 = pVertexData[Index1].pos - pVertexData[Index0].pos;
		glm::vec3 v2 = pVertexData[Index2].pos - pVertexData[Index0].pos;
		glm::vec3 Normal = glm::normalize( glm::cross(v1, v2) );

		pVertexData[Index0].normal += Normal;
		pVertexData[Index1].normal += Normal;
		pVertexData[Index2].normal += Normal;
	}

	// Normalize all the vertex normals
	for (GLsizei i = 0; i < VertexCount; i++)
	{ 
		pVertexData[i].normal = glm::normalize(pVertexData[i].normal);
	}
}