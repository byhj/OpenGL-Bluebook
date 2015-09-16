#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"

namespace byhj
{

class Triangle
{
public:
	Triangle();
	~Triangle();

	void Init();
	void Render();
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	ogl::Shader TriangleShader{ "Triangle Shader" };
	GLuint program = 0;
};

}
#endif
