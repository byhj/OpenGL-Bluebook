#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/ktx.h"

namespace byhj
{

class Texture
{
public:
	Texture();
	~Texture();

	void Init();
	void Render();
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	Shader TextureShader = { "Texture Shader" };

	GLuint program    = byhj::OGL_VALUE;
	GLuint tex_loc    = byhj::OGL_VALUE;
	GLuint mvp_loc    = byhj::OGL_VALUE;
	GLuint offset_loc = byhj::OGL_VALUE;
	GLuint tex = 0;

};

}
#endif
