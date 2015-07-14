#include <gl/glew.h>
#include "ogl/oglApp.h"
#include "ogl/shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

char *vertName[3] = {"triangle.vert", "quad.vert", "isoline.vert" };
char *tcsName[3]  =  {"triangle.tcs", "quad.tcs", "isoline.tcs" };
char *tesName[3]  =  {"triangle.tes", "quad.tes", "isoline.tes" };
char *fragName[3] =  {"triangle.frag", "quad.frag", "isoline.frag" };

class IndexCube: public byhj::Application
{
public:
	IndexCube():program_index(0) {};
	~IndexCube(){}

	void v_Init();
	void v_Render();
	void init_shader();
	void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (key == GLFW_KEY_M && action == GLFW_PRESS)
			program_index = (++program_index % 3) ;
	};
public:
	GLuint  program[3], vao;
	GLuint  program_index;
	Shader TessellationShader[3];
};

void IndexCube::v_Init()
{
	init_shader();
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void IndexCube::v_Render()
{
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, green);

	glUseProgram(program[program_index]);
	glDrawArrays(GL_PATCHES, 0, 4);
}

void IndexCube::init_shader()
{
	for (int i = 0; i != 3 ; ++i) 
	{
		TessellationShader[i].init();
		TessellationShader[i].attach(GL_VERTEX_SHADER, vertName[i]);
		TessellationShader[i].attach(GL_TESS_CONTROL_SHADER, tcsName[i]);
		TessellationShader[i].attach(GL_TESS_EVALUATION_SHADER, tesName[i]);
		TessellationShader[i].attach(GL_FRAGMENT_SHADER, fragName[i]);
		TessellationShader[i].link();
		program[i] = TessellationShader[i].GetProgram();
	}
}

CALL_MAIN(IndexCube);


