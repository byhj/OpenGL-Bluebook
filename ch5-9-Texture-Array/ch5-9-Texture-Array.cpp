#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ktx.cpp>

// Random number generator
static unsigned int seed = 0x13371337;

static inline float random_float()
{
	float res;
	unsigned int tmp;
	seed *= 16807;
	tmp = seed ^ (seed >> 4) ^ (seed << 15);
	*((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;
	return (res - 1.0f);
}

class Texture :public sb6::Application
{
public:
	Texture():TextureShader("Texture shader"), program(0), vao(0), ubo(0){}
	~Texture(){}
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();

	virtual void init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		init_shader();
		init_buffer();
		init_vertexArray();
		init_texture();
		for (int i = 0; i < 256; i++)
		{
			droplet_x_offset[i] = random_float() * 2.0f - 1.0f;
			droplet_rot_speed[i] = (random_float() + 0.5f) * ((i & 1) ? -3.0f : 3.0f);
			droplet_fall_speed[i] = random_float() + 0.2f;
		}
	}

	virtual void render()
	{
		static const GLfloat green[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);
		float currTime = glfwGetTime();
		glUseProgram(program);
		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D_ARRAY, tex_alien_array);

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);   //ubo使用映射方式
		glm::vec4 *droplet = (glm::vec4 *)glMapBufferRange(GL_UNIFORM_BUFFER, 
			                  0,  256 * sizeof(glm::vec4), 
							  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		for (int i = 0; i <256; ++i)
		{
			droplet[i].x = droplet_x_offset[i];
			droplet[i].y = 2.0f - fmodf((currTime + float(i)) * droplet_fall_speed[i], 4.31f);
			droplet[i].z = currTime * droplet_rot_speed[i];
			droplet[i].w = 0.0f;
		}
		glUnmapBuffer(GL_UNIFORM_BUFFER);

		int alien_index;
		for (alien_index = 0; alien_index != 256; ++alien_index)
		{
			glVertexAttribI1i(0, alien_index);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}
private:
	GLuint program, vao, ubo;
	Shader TextureShader;
	GLuint tex_alien_array, tex_loc;
protected:
	float droplet_x_offset[256]; //下落的参数
	float droplet_rot_speed[256]; 
	float droplet_fall_speed[256];
};

void Texture::init_texture()
{
	tex_alien_array = ktx::file::load("../media/textures/aliens.ktx");
	glBindTexture(GL_TEXTURE_2D_ARRAY, tex_alien_array);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}


void Texture::init_shader()
{
	TextureShader.attach(GL_VERTEX_SHADER, "texture.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "texture.frag");

	TextureShader.link();
	program = TextureShader.program;
	glUseProgram(program);
	tex_loc = glGetUniformLocation(program, "tex_aliens");
	glUniform1i(tex_loc, 0);
}


void Texture::init_buffer()
{
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
}


DECLARE_MAIN(Texture);