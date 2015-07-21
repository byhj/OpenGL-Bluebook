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
	windowInfo.title += "Window";
}

void RenderSystem::v_Init()
{
	m_Triangle.Init();
}

void RenderSystem::v_Render()
{

	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);

	m_Triangle.Render();

}

void RenderSystem::v_Shutdown()
{
	m_Triangle.Shutdown();
}

void RenderSystem::v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//Enter key 'F' to change the color smooth or flat
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		m_Triangle.ChangeFlat();
}


}