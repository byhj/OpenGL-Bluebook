#include <GL/glew.h>
#include "ogl/oglApp.h"
#include "ogl/shader.h"

class TextureApp: public byhj::Application
{
public:
	TextureApp():program(0), TriangleShader("Triangle Shader") 
	{
	};

	~TextureApp() {} ;

	void v_Init()
	{
		init_texture();
		init_shader();
	}

	void v_Render()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(program);

		//We use the texture we bind, the layer is 0 for uniform
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glUseProgram(0);
	}

	void v_Shutdown()
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

CALL_MAIN(TextureApp);

void TextureApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "texture.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "texture.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");
	
	//When you set the uniform value, you should use shader program first; 
	glUseProgram(program);
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

void TextureApp::init_texture()
{
	//GL_RGBA is input's style, GL_RGBA32F is show's style
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 256, 256);

	float *data  = new float[256 * 256 * 4];

	generate_texture(data, 256, 256);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_FLOAT, data);
	glBindTexture(GL_TEXTURE, 0);

	delete [] data;
}