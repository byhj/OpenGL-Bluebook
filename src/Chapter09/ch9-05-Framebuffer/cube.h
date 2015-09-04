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
	void init_fbo();

	GLuint   fbo_prog;
	GLuint   base_prog;

	GLuint   position_buffer;
	GLuint   index_buffer;
	GLuint   fbo, vao;
	GLuint   color_texture;
	GLuint   depth_texture;

	GLint    fbo_mv_loc, fbo_proj_loc;
	GLint    base_mv_loc, base_proj_loc;
	ogl::Shader FboShader;
	ogl::Shader BaseShader;

};

}
#endif
