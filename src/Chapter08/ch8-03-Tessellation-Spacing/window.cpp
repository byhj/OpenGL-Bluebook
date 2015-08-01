#include "window.h"

namespace byhj
{

Window::Window()
{

}

Window::~Window()
{

}

void Window::Init()
{
	init_shader();
}

void Window::Render()
{

	glUseProgram(program[program_index % 3]);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUniform1f(0, 5.3f);
	glDrawArrays(GL_PATCHES, 0, 4);

	glUseProgram(0);

}

void Window::Shutdown()
{
	glDeleteProgram(program[0]);
	glDeleteProgram(program[1]);
	glDeleteProgram(program[2]);
}

char *tesName[3] =
{
	"triangle.tes", "triangleFragEven.tes", "triangleFragOdd.tes"
};

void Window::init_shader()
{
	for (int i = 0; i != 3; ++i)
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

}