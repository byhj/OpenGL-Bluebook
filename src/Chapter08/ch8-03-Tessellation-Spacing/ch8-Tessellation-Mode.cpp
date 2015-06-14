#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6/sb6.h>
#include <sb6/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class IndexCube: public byhj::Application
{
public:
	IndexCube():program_index(0) {};
	~IndexCube(){}

	void v_Init();
	void v_Render();
	void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
	{	
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
			program_index = ++program_index % 3;
	}
	void init_shader();
public:
	GLuint  program[3], vao;
	GLuint  program_index;
	Shader  TriangleShader[3];
};

char *tesName[3] = 
{
	"triangle.tes", "triangleFragEven.tes", "triangleFragOdd.tes"
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
	glUniform1f(0, 5.3f);
	glUseProgram(program[program_index]);
	glDrawArrays(GL_PATCHES, 0, 4);

}

void IndexCube::init_shader()
{
	for (int i = 0; i != 3 ; ++i) 
	{
		TriangleShader[i].init();
		TriangleShader[i].attach(GL_VERTEX_SHADER, "triangle.vert");
		TriangleShader[i].attach(GL_TESS_CONTROL_SHADER, "triangle.tcs");
		TriangleShader[i].attach(GL_TESS_EVALUATION_SHADER, tesName[i]);
		TriangleShader[i].attach(GL_FRAGMENT_SHADER, "triangle.frag");
		TriangleShader[i].link();
		program[i] = TriangleShader[i].GetProgram();
	}
}

CALL_MAIN(IndexCube);

