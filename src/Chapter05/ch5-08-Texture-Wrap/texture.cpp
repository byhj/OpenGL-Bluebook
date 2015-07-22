#include "Texture.h"
#include "ogl/vmath.h"

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

	//left bottom   right bottom    left top            right top
	static const GLenum wrapModes[] = { GL_CLAMP_TO_EDGE,  GL_REPEAT,  GL_CLAMP_TO_BORDER,  GL_MIRRORED_REPEAT };
	static const GLfloat offsets[] =
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		-0.5f,  0.5f,
		0.5f,  0.5f
	};

	//Set the wrap border color
	static const GLfloat blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, blue);

	for (int i = 0; i != 4; ++i)
	{
		//We move the rectangle to four corner
		glUniform2fv(offset_loc, 1, &offsets[i * 2]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModes[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModes[i]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glUseProgram(0);
}


void Texture::Shutdown()
{
	glDeleteProgram(program);
	glDeleteTextures(1, &tex);
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
	TextureShader.attach(GL_VERTEX_SHADER,   "wrap.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "wrap.frag");
	TextureShader.link();
	program = TextureShader.GetProgram();

	tex_loc = glGetUniformLocation(program, "tex");
	assert(tex_loc != byhj::OGL_VALUE);
	offset_loc = glGetUniformLocation(program, "offset");
	assert(offset_loc != byhj::OGL_VALUE);
}


void Texture::init_texture()
{
	tex = sb6::ktx::load("../../../media/textures/rightarrows.ktx");
}

}