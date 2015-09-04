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
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint      prog_noise;
	GLuint      vao;
	int         loc_time;
	ogl::Shader NoiseShader;
};

}
#endif
