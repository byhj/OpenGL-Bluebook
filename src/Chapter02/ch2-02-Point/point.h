#ifndef Point_H
#define Point_H

#include <GL/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

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

	GLuint program = byhj::OGL_VALUE;
	GLuint vao     = byhj::OGL_VALUE;
	GLuint vbo     = byhj::OGL_VALUE;
	byhj::Shader PointShader = { "PointShader" };
};

}
#endif
