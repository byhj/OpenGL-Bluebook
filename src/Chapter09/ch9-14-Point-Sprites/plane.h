#ifndef Plane_H
#define Plane_H

#include <GL/glew.h>
#include "ogl/oglShader.h"

namespace byhj
{

class Plane
{
public:
	Plane();
	~Plane();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint   time_loc, proj_loc, texture;
	Shader   StarShader;
	GLuint   program;
	GLuint   star_texture;
	GLuint   star_vao;
	GLuint   star_buffer;
};

}
#endif
