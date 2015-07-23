#include "window.h"

#include <glfw/glfw3.h>

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
	glUseProgram(program);

	int i = static_cast<int>(glfwGetTime());

	//Notice: You should call this func after glUseProgram, we change subFunc every second
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutines[i & 1]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(0);
}

void Window::Shutdown()
{

}

void Window::init_shader()
{
	subShader.init();
	subShader.attach(GL_VERTEX_SHADER, "subroutine.vert");
	subShader.attach(GL_FRAGMENT_SHADER, "subroutine.frag");
	subShader.link();
	program = subShader.GetProgram();

	//Get the subFunc index in fragment shader
	subroutines[0] = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "myFunction1");
	subroutines[1] = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "myFunction2");
	sub_loc = glGetSubroutineUniformLocation(program, GL_FRAGMENT_SHADER, "subroutineUniform");
}

}