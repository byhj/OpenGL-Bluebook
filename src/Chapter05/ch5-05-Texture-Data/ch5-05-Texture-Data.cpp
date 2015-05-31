#include <GL/glew.h>
#include <sb6/sb6.h>
#include <sb6/shader.h>

class TriangleApp: public sb6::Application
{
public:
	TriangleApp():program(0), TriangleShader("Triangle Shader") 
	{
	};

	~TriangleApp() {} ;

	void vInit()
	{
		init_texture();
		init_shader();
	}

	void vRender()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(program);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glUseProgram(0);
	}

	void vShutdown()
	{
		glDeleteProgram(program);
	}

	void init_shader();
	void init_texture();

private:
	Shader TriangleShader;
	GLuint program;
	GLuint texture, tex_loc;
};

DECLARE_MAIN(TriangleApp);

void TriangleApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "texture.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "texture.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");
	glUniform1i(tex_loc, 0);
}

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

void TriangleApp::init_texture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 256, 256);
	float *data  = new float[256 * 256 * 4];
	generate_texture(data, 256, 256);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_FLOAT, data);
	glBindTexture(GL_TEXTURE, 0);

	delete [] data;
}