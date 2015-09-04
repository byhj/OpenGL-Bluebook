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
	void Render(GLfloat aspect, GLuint sw, GLuint sh);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	GLuint program;
	GLuint vao;
	GLuint position_buffer;
	GLuint index_buffer;
	GLuint uniform_buffer;
	GLint  mv_location;
	GLint  proj_location;
	ogl::Shader viewShader;

};

}
#endif
