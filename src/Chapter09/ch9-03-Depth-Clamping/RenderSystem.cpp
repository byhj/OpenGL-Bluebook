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
	windowInfo.title += "Depth Clamping";
}

void RenderSystem::v_Init()
{
	glEnable(GL_DEPTH_TEST);

	m_Plane.Init();
}

void RenderSystem::v_Render()
{

	static const GLfloat black[] = { 0.2f, 0.3f, 0.4f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	static const GLfloat one[] = { 1.0f };
	glClearBufferfv(GL_DEPTH, 0, one);

	m_Plane.Render(GetAspect());

}

void RenderSystem::v_Shutdown()
{
	m_Plane.Shutdown();
}
}