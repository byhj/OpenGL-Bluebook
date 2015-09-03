#ifndef Plane_H
#define Plane_H

#include <GL/glew.h>
#include "ogl/Shader.h"

namespace byhj
{

class Plane
{
public:
	Plane();
	~Plane();

	void Init();
	void Render();
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint exposure_loc;
	Shader exposureShader;
	GLuint  tex_src;
	GLuint  tex_lut;
	GLuint  program_exposure;
	GLuint  vao;
	float   exposure;
	int     mode;
};

}
#endif
