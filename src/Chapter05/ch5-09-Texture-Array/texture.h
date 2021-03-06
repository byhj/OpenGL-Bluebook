#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include <array>

#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/ktx.h"

const int TEX_NUM = 256;

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

	ogl::Shader TextureArrayShader = { "TextureArray Shader" };

	GLuint program = ogl::VALUE;
	GLuint tex_loc = ogl::VALUE;
	GLuint mvp_loc = ogl::VALUE;
	GLuint vao = ogl::VALUE;
	GLuint ubo = ogl::VALUE;

	GLuint tex_alien_array = 0;

	//Ubo data struct
    std::array<float, TEX_NUM> droplet_x_offset;
	std::array<float, TEX_NUM> droplet_rot_speed;
	std::array<float, TEX_NUM> droplet_fall_speed;

};

}
#endif
