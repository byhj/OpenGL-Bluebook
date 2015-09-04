#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>

#include "ogl/Shader.h"
#include "ogl/Utility.h"
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

	ogl::Shader TextureShader = { "Texture Shader" };

	GLuint program    = ogl::VALUE;
	GLuint tex_loc    = ogl::VALUE;
	GLuint mvp_loc    = ogl::VALUE;
	GLuint offset_loc = ogl::VALUE;
	GLuint tex = 0;

};

}
#endif
