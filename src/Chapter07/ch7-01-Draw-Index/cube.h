#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"

namespace byhj
{

class Cube
{
public:
	Cube();
	~Cube();

	void Init();
	void Render(const ogl::MvpMatrix &matrix);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	ogl::ogl::Shader cubeShader = { "Cube Shader" };

	GLuint program = ogl::VALUE;
	GLuint mvp_loc = ogl::VALUE;
	GLuint vao = ogl::VALUE;
	GLuint ibo = ogl::VALUE;
	GLuint vbo = ogl::VALUE;

};

}
#endif
