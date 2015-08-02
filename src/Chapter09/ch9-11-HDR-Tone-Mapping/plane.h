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
	void Render();
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint exposure_loc;
	Shader naiveShader, exposureShader, adaptiveShader;
	GLuint      tex_src;
	GLuint      tex_lut;
	GLuint      program_naive;
	GLuint      program_exposure;
	GLuint      program_adaptive;
	GLuint      vao;
	float       exposure;
	int         mode;
};

}
#endif
