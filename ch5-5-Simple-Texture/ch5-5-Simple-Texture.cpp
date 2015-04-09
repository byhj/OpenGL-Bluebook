#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Texture :public sb6::Application
{
public:
	Texture():TextureShader("Texture shader"), program(0), vao(0), vbo(0){}
	~Texture(){}
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();

	virtual void init()
	{
		init_shader();
		init_texture();
	}

	virtual void render()
	{
		static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);
		glUseProgram(program);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}
private:
	GLuint program, vao, vbo;
	Shader TextureShader;
	GLuint mvp_loc, tex_loc, tex;
};



void generate_texture(float *data, int width, int height)
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
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 256, 256);
	float *data = new float [256 * 256 * 4];
	generate_texture(data, 256, 256);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,256, 256, GL_RGBA, GL_FLOAT, data);
	delete [] data;
}


void Texture::init_shader()
{
	TextureShader.attach(GL_VERTEX_SHADER, "texture.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "texture.frag");

	TextureShader.link();
	program = TextureShader.program;
	glUseProgram(program);
	tex_loc = glGetUniformLocation(program, "tex");
	glUniform1i(tex_loc, 0);
}


void Texture::init_buffer()
{
}

void Texture::init_vertexArray()
{
}


DECLARE_MAIN(Texture);