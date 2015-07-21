#ifndef Point_H
#define Point_H

#include <GL/glew.h>
#include "ogl/oglShader.h"

namespace byhj
{

class Point
{
public:
	Point();
	~Point();

	void Init();
	void Render();
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	GLuint program = -1;
	GLuint vao = -1;
	GLuint vbo = -1;
	Shader PointShader = { "PointShader" };
};

}
#endif
