#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ktx.cpp>

class KtxView :public sb6::Application
{
public:
	KtxView(){}
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
		static const GLfloat black[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		glUseProgram(program_exposure);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_src);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	virtual void shutdown()
	{

	}

private:
	GLuint exposure_loc ;
	Shader     exposureShader;
	GLuint      tex_src;
	GLuint      tex_lut;
	GLuint      program_exposure;
	GLuint      vao;
	float       exposure;
	int         mode;
};


void KtxView::init_texture()
{
	tex_src = ktx::file::load("../media/textures/treelights_2k.ktx");
	glBindTexture(GL_TEXTURE_2D, tex_src);
}


void KtxView::init_shader()
{


	exposureShader.attach(GL_VERTEX_SHADER, "hdr.vert");
	exposureShader.attach(GL_FRAGMENT_SHADER, "hdr_exposure.frag");
	exposureShader.link();
	program_exposure = exposureShader.program;
	exposure_loc = glGetUniformLocation(program_exposure, "exposure");
}


void KtxView::init_buffer()
{

}

void KtxView::init_vertexArray()
{
}

DECLARE_MAIN(KtxView);