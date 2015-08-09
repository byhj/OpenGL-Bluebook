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
	windowInfo.title += "Geometry Primitive";
}

void RenderSystem::v_Init()
{

	glEnable(GL_DEPTH_TEST);

	m_Geometry.Init();
}

void RenderSystem::v_Render()
{

	static const GLfloat black[] = { 0.2f, 0.3f, 0.4f, 1.0f };
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