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
	windowInfo.title += "Tessellation Cube";
}

void RenderSystem::v_Init()
{

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_Cube.Init();
}

void RenderSystem::v_Render()
{

	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	static const GLfloat one[] = { 1.0f };
	glClearBufferfv(GL_DEPTH, 0, one);

	m_Cube.Render(GetAspect());

}

void RenderSystem::v_Shutdown()
{
	m_Cube.Shutdown();
}
}