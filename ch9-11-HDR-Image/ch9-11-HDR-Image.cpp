#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ktx.cpp>

class KtxView :public sb6::Application
{
public:
	KtxView():KtxViewShader("KtxView shader"), program(0)
	{
	}
	~KtxView(){}
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

		float t = glfwGetTime();
		glUseProgram(program);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1f(exposure_loc, (float)(sin(t) * 16.0 + 16.0));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}

private:
	GLuint program;
	Shader KtxViewShader;
	GLuint tex_loc, texture, exposure_loc;
};


void KtxView::init_texture()
{
	glGenTextures(1, &texture);
	texture = ktx::file::load("../media/textures/tree.ktx");
	glBindTexture(GL_TEXTURE_2D, texture);
}


void KtxView::init_shader()
{
	KtxViewShader.attach(GL_VERTEX_SHADER, "hdr.vert");
	KtxViewShader.attach(GL_FRAGMENT_SHADER, "hdr.frag");
	KtxViewShader.link();
	program = KtxViewShader.program;

	tex_loc = glGetUniformLocation(program, "s");
	exposure_loc = glGetUniformLocation(program, "exposure");
	glUniform1i(tex_loc, 0);
}


void KtxView::init_buffer()
{

}

void KtxView::init_vertexArray()
{
}

DECLARE_MAIN(KtxView);