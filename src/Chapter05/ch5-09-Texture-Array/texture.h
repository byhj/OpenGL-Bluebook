#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include <array>

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/ktx.h"

constexpr int TEX_NUM = 256;

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
	void update();
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	Shader TextureArrayShader = { "TextureArray Shader" };

	GLuint program = byhj::OGL_VALUE;
	GLuint tex_loc = byhj::OGL_VALUE;
	GLuint mvp_loc = byhj::OGL_VALUE;
	GLuint vao = byhj::OGL_VALUE;
	GLuint ubo = byhj::OGL_VALUE;

	GLuint tex_alien_array = 0;

	//Ubo data struct
    std::array<float, TEX_NUM> droplet_x_offset;
	std::array<float, TEX_NUM> droplet_rot_speed;
	std::array<float, TEX_NUM> droplet_fall_speed;

};

}
#endif
