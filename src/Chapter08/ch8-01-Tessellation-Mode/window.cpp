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
	glDrawArrays(GL_PATCHES, 0, 4);

	glUseProgram(0);

}

void Window::Shutdown()
{
	glDeleteProgram(program[0]);
	glDeleteProgram(program[1]);
	glDeleteProgram(program[2]);
}

char *vertName[3] ={ "triangle.vert", "quad.vert", "isoline.vert" };
char *tcsName[3]  ={ "triangle.tcs", "quad.tcs", "isoline.tcs" };
char *tesName[3]  ={ "triangle.tes", "quad.tes", "isoline.tes" };
char *fragName[3] ={ "triangle.frag", "quad.frag", "isoline.frag" };

void Window::init_shader()
{
	for (int i = 0; i != 3; ++i)
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

}