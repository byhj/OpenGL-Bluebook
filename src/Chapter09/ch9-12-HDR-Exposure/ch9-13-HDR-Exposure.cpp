#include <gl/glew.h>
#include <sb6/sb6.h>
#include <sb6/shader.h>
#include <sb6/ktx.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class KtxView :public byhj::Application
{
public:
	KtxView(){}
	~KtxView(){}
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();

	void v_Init()
	{
		init_shader();
		init_texture();
	}

	void v_Render()
	{
		static const GLfloat black[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		glUseProgram(program_exposure);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_src);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
    

private:
	GLuint exposure_loc ;
	Shader exposureShader;
	GLuint  tex_src;
	GLuint  tex_lut;
	GLuint  program_exposure;
	GLuint  vao;
	float   exposure;
	int     mode;
};


void KtxView::init_texture()
{
	tex_src = sb6::ktx::load("../../../media/textures/treelights_2k.ktx");
	glBindTexture(GL_TEXTURE_2D, tex_src);
	
}


void KtxView::init_shader()
{
	exposureShader.attach(GL_VERTEX_SHADER, "hdr.vert");
	exposureShader.attach(GL_FRAGMENT_SHADER, "hdr_exposure.frag");
	exposureShader.link();
	program_exposure = exposureShader.GetProgram();
	exposure_loc = glGetUniformLocation(program_exposure, "exposure");
}


void KtxView::init_buffer()
{
}

void KtxView::init_vertexArray()
{
}

CALL_MAIN(KtxView);