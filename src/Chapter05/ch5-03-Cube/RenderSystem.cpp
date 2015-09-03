#include <GL/glew.h>

#include "ogl/Utility.h"
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
	windowInfo.title += "Cube";
}

void RenderSystem::v_Init()
{
	m_Cube.Init();
}

void RenderSystem::v_Render()
{

	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	static const GLfloat one[] = { 1.0f };
	glClearBufferfv(GL_DEPTH, 0, one);

	static byhj::MvpMatrix matrix;
	float time = static_cast<float>( glfwGetTime() );
	matrix.model = glm::rotate(glm::mat4(1.0f), time, glm::vec3(1.0f, 1.0f, 1.0f) );
	matrix.view  = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	matrix.proj  = glm::perspective(45.0f, GetAspect(), 1.0f, 1000.0f);

	m_Cube.Render(matrix);

}

void RenderSystem::v_Shutdown()
{
	m_Cube.Shutdown();
}
}