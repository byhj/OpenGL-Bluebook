#include <GL/glew.h>
#include "RenderSystem.h"

namespace byhj
{

RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::v_InitInfo()
{
	windowInfo.title += "Tessellation Mode";
}

void RenderSystem::v_Init()
{
	m_Window.Init();
}

void RenderSystem::v_Render()
{
	const GLfloat bgColor[] = {0.2f, 0.3f, 0.4f, 1.0f};

	glClearBufferfv(GL_COLOR, 0, bgColor);

	m_Window.Render();

}

void RenderSystem::v_Shutdown()
{
	m_Window.Shutdown();
}

void RenderSystem::v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		m_Window.ChangeProgram();
}

}