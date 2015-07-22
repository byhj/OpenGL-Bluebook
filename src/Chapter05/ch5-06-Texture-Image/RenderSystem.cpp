#include <GL/glew.h>
#include "ogl/oglUtility.h"
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_Texture.Init();
}

void RenderSystem::v_Render()
{

	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat ones[] = { 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, ones);

	float currentTime = static_cast<float>(glfwGetTime());

	static byhj::MvpMatrix matrix;
	matrix.proj  = glm::perspective(45.0f,GetAspect(), 0.1f, 1000.0f);
	matrix.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	matrix.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))
		         * glm::rotate(glm::mat4(1.0f), glm::radians(currentTime * 19.3f), glm::vec3(0.0f, 1.0f, 0.0f))
		         * glm::rotate(glm::mat4(1.0f), glm::radians(currentTime * 21.1f), glm::vec3(0.0f, 0.0f, 1.0f));

	m_Texture.Render(matrix);

}

void RenderSystem::v_Shutdown()
{
	m_Texture.Shutdown();
}

void RenderSystem::v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		m_Texture.ChangeTexture();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


}