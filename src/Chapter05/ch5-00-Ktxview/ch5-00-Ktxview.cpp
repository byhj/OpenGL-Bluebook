#include <GL/glew.h>
#include "ogl/oglApp.h"
#include "ogl/ktx.cpp"
#include "ogl/shader.h"
#include "ogl/glDebug.h"

class simpletexture_app : public byhj::Application
{
public:
    void v_Init()
    {
		init_shader();
		init_texture();

    }

    void v_Shutdown(void)
    {
        glDeleteProgram(program);
        glDeleteVertexArrays(1, &vao);
        glDeleteTextures(1, &texture);
    }

    void v_Render()
    {
		glUseProgram(program);

		float t = glfwGetTime();
        static const GLfloat bgColor[] = { 0.2f, 0.3f, 0.4f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, bgColor);

		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1f(exposure_loc, (float)(sin(t) * 16.0 + 16.0));

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glUseProgram(0);
    }

private:
	void init_shader();
	void init_texture();

    GLuint      texture;
    GLuint      program;
    GLuint      vao;
	GLuint      exposure_loc;
	Shader      KtxShader;
};

CALL_MAIN(simpletexture_app);

void simpletexture_app::init_shader()
{
	KtxShader.init();
	KtxShader.attach(GL_VERTEX_SHADER, "ktx.vert");
	KtxShader.attach(GL_FRAGMENT_SHADER, "ktx.frag");
	KtxShader.link();
	program = KtxShader.GetProgram();
    exposure_loc = glGetUniformLocation(program, "exposure");
}

void simpletexture_app::init_texture()
{
	glGenTextures(1, &texture);
	sb6::ktx::load("../../../media/textures/tree.ktx", texture);
	glBindTexture(GL_TEXTURE_2D, 0);
}