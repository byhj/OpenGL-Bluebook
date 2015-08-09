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
	windowInfo.title += "Shader SubRoutines";
}

void RenderSystem::v_Init()
{
	m_Window.Init();
}

void RenderSystem::v_Render()
{

	float bgColor[4] ={ 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, bgColor);

	m_Window.Render();

}

void RenderSystem::v_Shutdown()
{
	m_Window.Shutdown();
}
}