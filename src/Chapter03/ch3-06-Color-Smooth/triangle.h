#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/oglShader.h"

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

	void ChangeFlat();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	Shader TriangleShader{ "Triangle Shader" };
	GLuint program = 0;
	GLboolean flat;
};

}
#endif
