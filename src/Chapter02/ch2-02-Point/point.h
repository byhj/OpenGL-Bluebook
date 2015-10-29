#ifndef Point_H
#define Point_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"

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

	GLuint program = ogl::VALUE;
	GLuint vao     = ogl::VALUE;
	GLuint vbo     = ogl::VALUE;
	ogl::Shader PointShader = { "PointShader" };
};

}
#endif
