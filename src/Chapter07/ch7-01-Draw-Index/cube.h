#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

namespace byhj
{

class Cube
{
public:
	Cube();
	~Cube();

	void Init();
	void Render(const byhj::MvpMatrix &matrix);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	Shader CubeShader = { "Cube Shader" };

	GLuint program = byhj::OGL_VALUE;
	GLuint mvp_loc = byhj::OGL_VALUE;
	GLuint vao = byhj::OGL_VALUE;
	GLuint ibo = byhj::OGL_VALUE;
	GLuint vbo = byhj::OGL_VALUE;

};

}
#endif
