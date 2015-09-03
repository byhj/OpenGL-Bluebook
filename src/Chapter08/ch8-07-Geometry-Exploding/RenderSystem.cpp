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
	windowInfo.title += "Geometry Exploding";
}

void RenderSystem::v_Init()
{

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_Geometry.Init();
}

void RenderSystem::v_Render()
{

	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	static const GLfloat one[] = { 1.0f };
	glClearBufferfv(GL_DEPTH, 0, one);

	m_Geometry.Render(GetAspect());

}

void RenderSystem::v_Shutdown()
{
	m_Geometry.Shutdown();
}
}