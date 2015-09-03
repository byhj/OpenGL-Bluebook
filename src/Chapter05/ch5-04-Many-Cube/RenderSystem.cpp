#include <GL/glew.h>

#include "ogl/Utility.h"
#include "RenderSystem.h"

#define MANY_CUBE 
const int CubeSize = 24;

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
	windowInfo.title += "Many Cube";
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

	float time = static_cast<float>(glfwGetTime());
	static byhj::MvpMatrix matrix;
	matrix.view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	matrix.proj = glm::perspective(45.0f, GetAspect(), 1.0f, 1000.0f);

#ifdef MANY_CUBE

	for (int i = 0; i != CubeSize; ++i)
	{
		float f = (float)i + time * 0.3f;

		matrix.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(-time * 45.0f), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(time * 21.0f), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::translate(glm::mat4(1.0f), glm::vec3(  sinf(2.1f * f) * 2.0f, cosf(1.7f * f) * 2.0f,
				                                          sinf(1.3f * f) * cosf(1.5f * f) * 2.0f)
				            );
		m_Cube.Render(matrix);
	}
#else

	matrix.model = glm::rotate(glm::mat4(1.0f), glm::radians(time), glm::vec3(1.0f, 1.0f, 1.0f));
	m_Cube.Render(matrix);

#endif

}

void RenderSystem::v_Shutdown()
{
	m_Cube.Shutdown();
}

}