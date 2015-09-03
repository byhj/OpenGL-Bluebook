#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"

namespace byhj
{

class Triangle
{
public:
	Triangle();
	~Triangle();

	void Init();
	void Render();
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint      texture = 0;
	GLuint      program = 0;
	GLuint      exposure_loc = 0;
	Shader      KtxShader = { "Ktx Shader" };
};

}
#endif
