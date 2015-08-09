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
	windowInfo.title += "HDR-Tone-Mapping";
}

void RenderSystem::v_Init()
{
	m_Plane.Init();
}

void RenderSystem::v_Render()
{

	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);

	m_Plane.Render();

}

void RenderSystem::v_Shutdown()
{
	m_Plane.Shutdown();
}


}