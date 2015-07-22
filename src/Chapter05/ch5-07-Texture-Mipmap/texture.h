#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include <array>

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/object.h"
#include "ogl/ktx.h"

const int TEX_SIZE = 4;

namespace byhj
{

class Texture
{
public:
	Texture();
	~Texture();

	void Init();
	void Render(const float &aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	Shader TextureShader = { "Texture Shader" };

	GLuint tex_index = 0;
	GLuint program    = byhj::OGL_VALUE;
	GLuint tex_loc    = byhj::OGL_VALUE;
	GLuint mvp_loc    = byhj::OGL_VALUE;
	GLuint offset_loc = byhj::OGL_VALUE;
	GLuint tex_wall, tex_ceiling, tex_floor;

	std::array<GLuint, TEX_SIZE> textures = { byhj::OGL_VALUE, byhj::OGL_VALUE };
};

}
#endif
