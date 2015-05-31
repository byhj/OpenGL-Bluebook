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
		init_shader();
	}

	void vRender()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(0);
	}

	void vShutdown()
	{
		glDeleteProgram(program);
	}

	void init_shader();

private:
	Shader TriangleShader;
	GLuint program;
};

DECLARE_MAIN(TriangleApp);

void TriangleApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
}