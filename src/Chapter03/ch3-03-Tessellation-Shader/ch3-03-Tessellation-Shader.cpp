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
	TriangleShader.attach(GL_TESS_CONTROL_SHADER, "triangle.tcs");
	TriangleShader.attach(GL_TESS_EVALUATION_SHADER, "triangle.tes");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
}
