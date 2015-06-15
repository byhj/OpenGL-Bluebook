#include <GL/glew.h>
#include <sb6/sb6.h>
#include <sb6/ktx.cpp>
#include <sb6/shader.h>
#include <sb6/glDebug.h>

class simpletexture_app : public byhj::Application
{
public:
    void v_Init()
    {
		init_shader();
		// Generate a name for the texture
		glGenTextures(1, &texture);
		// Load texture from file
		sb6::ktx::load("../../../media/textures/tree.ktx", texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		
    }

    void v_Shutdown(void)
    {
        glDeleteProgram(program);
        glDeleteVertexArrays(1, &vao);
        glDeleteTextures(1, &texture);
    }

    void v_Render()
    {
		float t = glfwGetTime();
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, green);

        glUseProgram(program);
		glBindTexture(GL_TEXTURE_2D, texture);
        glViewport(0, 0, GetScreenWidth(), GetScreenHeight());
        glUniform1f(0, (float)(sin(t) * 16.0 + 16.0));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

private:
	void init_shader();

    GLuint      texture;
    GLuint      program;
    GLuint      vao;
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
}