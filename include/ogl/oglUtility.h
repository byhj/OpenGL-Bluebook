#ifndef OGLUTILITY_H
#define OGLUTILITY_H

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>

namespace byhj
{
	struct MvpMatrix
	{
	   glm::mat4 model;
	   glm::mat4 view;
	   glm::mat4 proj;
	};

	struct Vertex
	{
		glm::vec3 pos; //���㣬������������
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

}

#define BUFFER_OFFSET(offset) ((GLvoid*)(NULL + offset))
#define ARRAY_ELEMENTS(a) ( sizeof(a)/sizeof(a[0]) )

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

void CalcNormals(const GLuint *pIndexData, GLsizei IndexCount,
				 byhj::Vertex *pVertexData, GLsizei VertexCount);

#endif
