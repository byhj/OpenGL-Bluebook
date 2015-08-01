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
	glBindVertexArray(0);

	update();

	glUniform1i(tex_loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, tex_alien_array);

	//We draw 256 aliens, send each index to shader use vertexAttrib*
	for (int alien_index = 0; alien_index != TEX_NUM; ++alien_index)
	{
		glVertexAttribI1i(0, alien_index);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glBindVertexArray(0);
	glUseProgram(0);
}

void Texture::Shutdown()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &ubo);
	glDeleteVertexArrays(1, &vao);
	glDeleteTextures(1, &tex_alien_array);
}

void Texture::update()
{
	//We change the ubo data every frame, use mapbuffer, 0 is bind point
	// GL_MAP_INVALIDATE_BUFFER_BIT: previous contents of the entire buffer may be discarded. 
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

	float time = static_cast<float>(glfwGetTime());

	glm::vec4 *droplet = (glm::vec4*)glMapBufferRange(
		                             GL_UNIFORM_BUFFER,
		                             0, 256 * sizeof(glm::vec4),
		                             GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	//Create the drop random data for texture show
	for (int i = 0; i < 256; ++i)
	{
		droplet[i].x = droplet_x_offset[i];
		droplet[i].y = 2.0f - fmodf((time + float(i)) * droplet_fall_speed[i], 4.31f);
		droplet[i].z = time * droplet_rot_speed[i];
		droplet[i].w = 1.0f;
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);
}


// Random number generator
static unsigned int seed = 0x13371337;

static inline float random_float()
{
	float res;
	unsigned int tmp;
	seed *= 16807;
	tmp = seed ^ (seed >> 4) ^ (seed << 15);
	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;
	return (res - 1.0f);
}

void Texture::init_buffer()
{
	for (int i = 0; i < 256; i++)
	{
		droplet_x_offset[i] = random_float() * 2.0f - 1.0f;
		droplet_rot_speed[i] = (random_float() + 0.5f) * ((i & 1) ? -3.0f : 3.0f);
		droplet_fall_speed[i] = random_float() + 0.2f;
	}

	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, 256 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Texture::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);

	glBindVertexArray(0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Texture::init_shader()
{
	TextureArrayShader.init();
	TextureArrayShader.attach(GL_VERTEX_SHADER, "textureArray.vert");
	TextureArrayShader.attach(GL_FRAGMENT_SHADER, "textureArray.frag");
	TextureArrayShader.link();
	program = TextureArrayShader.GetProgram();

	tex_loc = glGetUniformLocation(program, "tex_array");
	assert(tex_loc != byhj::OGL_VALUE);
}


void Texture::init_texture()
{
	tex_alien_array = sb6::ktx::load("../../../media/textures/aliens.ktx");
	glBindTexture(GL_TEXTURE_2D_ARRAY, tex_alien_array);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

}