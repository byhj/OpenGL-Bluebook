#include <GL/glew.h>
#include "ogl/oglApp.h"
#include <ogl/shader.h>

class TriangleApp: public byhj::Application
{
public:
	TriangleApp():program(0), TriangleShader("Triangle Shader") 
	{
	};

	~TriangleApp() {} ;

	void v_Init()
	{
		init_shader();
	}

	void v_Render()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, black);

		//We use current time change the vertex position every frame,
		//it will animate just a cricle.
		GLfloat time = static_cast<GLfloat>(glfwGetTime());
		GLfloat offset[] = {
			(float)sin(time) * 0.5f, 
			(float)cos(time) * 0.5f,
			0.0f, 0.0f
		};

		glUseProgram(program);
		glVertexAttrib4fv(0, offset);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_PATCHES, 0, 3);
		glUseProgram(0);
	}

	void v_Shutdown()
	{
		glDeleteProgram(program);
	}

	void init_shader();

private:
	Shader TriangleShader;
	GLuint program;
};

CALL_MAIN(TriangleApp);

void TriangleApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_TESS_CONTROL_SHADER, "triangle.tcs");
	TriangleShader.attach(GL_TESS_EVALUATION_SHADER, "triangle.tes");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
}
