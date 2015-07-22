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

	textures[0] = tex_wall; textures[1] = tex_floor;
	textures[2] = tex_wall; textures[3] = tex_ceiling;
}

void Texture::Render(const float &aspect)
{
	glUseProgram(program);

	glUniform1i(tex_loc, 0);

	float currentTime = static_cast<float>(glfwGetTime());
	glUniform1f(offset_loc, currentTime * 0.003f);

	vmath::mat4 proj = vmath::perspective(60.0f, aspect, 0.1f, 100.0f);

	for (int i = 0; i != 4; ++i)
	{
		vmath::mat4 mv = vmath::rotate(90.0f * (float)i, vmath::vec3(0.0f, 0.0f, 1.0f))
			           * vmath::translate(-0.5f, 0.0f, -10.0f)
		               * vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f)
		               * vmath::scale(30.0f, 1.0f, 1.0f);

		vmath::mat4 mvp = proj * mv;
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glUseProgram(0);
}


void Texture::Shutdown()
{
	glDeleteProgram(program);
	glDeleteTextures(1, &textures[0]);
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
	TextureShader.attach(GL_VERTEX_SHADER, "tunnel.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "tunnel.frag");
	TextureShader.link();
	program = TextureShader.GetProgram();

	tex_loc = glGetUniformLocation(program, "tex");
	assert(tex_loc != byhj::OGL_VALUE);

	mvp_loc = glGetUniformLocation(program, "mvp");
	assert(mvp_loc != byhj::OGL_VALUE);

	offset_loc = glGetUniformLocation(program, "offset");
	assert(offset_loc != byhj::OGL_VALUE);
}


void Texture::init_texture()
{
	tex_wall = sb6::ktx::load("../../../media/textures/brick.ktx");
	tex_ceiling = sb6::ktx::load("../../../media/textures/ceiling.ktx");
	tex_floor = sb6::ktx::load("../../../media/textures/floor.ktx");

	GLuint texture[] = { tex_floor, tex_wall, tex_ceiling };

	for (int i = 0; i != 3; ++i)
	{
		//Set each texture parameteri
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

}