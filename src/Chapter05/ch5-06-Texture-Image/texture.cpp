#include "Texture.h"
#include <glfw/glfw3.h>

namespace byhj
{

Texture::Texture()
{

}

Texture::~Texture()
{

}

void Texture::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Texture::Render(const ogl::MvpMatrix &matrix)
{
	glUseProgram(program);

	glUniform1i(tex_loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[tex_index]);

	glm::mat4 model = matrix.model;
	glm::mat4 view  = matrix.view;
	glm::mat4 proj  = matrix.proj;
	glm::mat4 mvp   = proj * view * model;
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

	TorusModel.render();

	glUseProgram(0);
}


void Texture::Shutdown()
{
	glDeleteProgram(program);
}

void Texture::ChangeTexture()
{
	tex_index = !tex_index;
}

void Texture::init_buffer()
{
	TorusModel.load("../../../media/objects/torus_nrms_tc.sbm");
}

void Texture::init_vertexArray()
{

}

void Texture::init_shader()
{
	TextureShader.init();
	TextureShader.attach(GL_VERTEX_SHADER, "texture.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "texture.frag");
	TextureShader.link();
	program = TextureShader.GetProgram();

	tex_loc = glGetUniformLocation(program, "tex");
	assert(tex_loc != ogl::VALUE);
	mvp_loc = glGetUniformLocation(program, "mvp");
	assert(mvp_loc != ogl::VALUE);

}

//Checkerbox texture
#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
static const GLubyte tex_data[] =
{
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
};
#undef B
#undef W

void Texture::init_texture()
{
	glGenTextures(1, &textures[0]);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 16, 16);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	textures[1] = sb6::ktx::load("../../../media/textures/pattern1.ktx");

	glBindTexture(GL_TEXTURE_2D, 0);
}

}