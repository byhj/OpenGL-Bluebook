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
	windowInfo.title += "Instance";
}

void RenderSystem::v_Init()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);

	m_Instance.Init();
}

void RenderSystem::v_Render()
{
			glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	static const GLfloat one[] = { 1.0f };
	glClearBufferfv(GL_DEPTH, 0, one);

	m_Instance.Render();

}

void RenderSystem::v_Shutdown()
{
	m_Instance.Shutdown();
}
}