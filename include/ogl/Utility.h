#ifndef UTILITY_H
#define UTILITY_H

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>

namespace byhj
{
namespace ogl
{

	struct MvpMatrix
	{
	   glm::mat4 model;
	   glm::mat4 view;
	   glm::mat4 proj;
	};

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

	const GLuint VALUE = 0xffffffff;
}

}
#define BUFFER_OFFSET(offset) ((GLvoid*)(NULL + offset))
#define ARRAY_ELEMENTS(a) ( sizeof(a)/sizeof(a[0]) )
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }


#endif
