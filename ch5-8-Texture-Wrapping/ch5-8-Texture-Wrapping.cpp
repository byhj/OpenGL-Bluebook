#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <object.cpp>
#include <ktx.cpp>

#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
static const GLubyte tex_data[] =  //棋盘黑白纹理
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

class Texture :public sb6::Application
{
public:
	Texture():TextureShader("Texture shader"), program(0){
		memset(keys, 0, sizeof(keys));
	}
	~Texture(){}
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();

	virtual void init()
	{
		init_shader();
		init_texture();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	virtual void render()
	{
		static const GLfloat gray[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		static const GLfloat ones[] = { 1.0f };
		glClearBufferfv(GL_COLOR, 0, gray);
		glClearBufferfv(GL_DEPTH, 0, ones);

		static const GLenum wrapModes[] = { GL_CLAMP_TO_EDGE, GL_REPEAT, 
			                                GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT};
		static const GLfloat offsets[] = { -0.5f, -0.5f,
			                                0.5f, -0.5f,
								           -0.5f,  0.5f,
									        0.5f,  0.5f
		                                 };
		static const GLfloat yellow[] = { 0.0f, 0.1f, 0.0f, 1.0f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, yellow);  //设置插值常数
		glUseProgram(program);
		for (int i = 0; i != 4; ++i)
		{
		   glUniform2fv(glGetUniformLocation(program, "offset"), 1, &offsets[i * 2]);
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModes[i]); //纹理环绕, 纹理坐标s，t
		   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModes[i]);
		   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}
	virtual void shotdown()
	{
		glDeleteProgram(program);
		glDeleteTextures(1, &tex);
	}
private:
	GLuint program;
	Shader TextureShader;
	GLuint  tex_loc, tex;
	bool keys[1024];
};


void Texture::init_texture()
{
	tex = ktx::file::load("../media/textures/rightarrows.ktx");
}


void Texture::init_shader()
{
	TextureShader.attach(GL_VERTEX_SHADER, "wrap.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "wrap.frag");

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