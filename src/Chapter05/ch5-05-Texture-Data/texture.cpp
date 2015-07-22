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

void Texture::Render()
{
	glUseProgram(program);

	glUniform1i(tex_loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(0);
}


void Texture::Shutdown()
{
	glDeleteProgram(program);
}

void Texture::init_buffer()
{

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
	assert(tex_loc != byhj::OGL_VALUE);

}

void Texture::generate_texture(float *data, int width, int height)
{
	int x, y;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			data[(y * width + x) * 4 + 0] = (float)((x & y) & 0xFF) / 255.0f;
			data[(y * width + x) * 4 + 1] = (float)((x | y) & 0xFF) / 255.0f;
			data[(y * width + x) * 4 + 2] = (float)((x ^ y) & 0xFF) / 255.0f;
			data[(y * width + x) * 4 + 3] = 1.0f;
		}
	}
}

void Texture::init_texture()
{
	//GL_RGBA is input's style, GL_RGBA32F is show's style
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 256, 256);

	float *data = new float[256 * 256 * 4];

	generate_texture(data, 256, 256);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_FLOAT, data);
	glBindTexture(GL_TEXTURE, 0);

	delete []  data;
	data = nullptr;
}

}