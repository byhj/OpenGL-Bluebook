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
	windowInfo.title += "Window";
}

void RenderSystem::v_Init()
{
	static const GLfloat color[] ={ 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, color);

	m_Plane.Init();

}

void RenderSystem::v_Render()
{


	m_Plane.Render(GetAspect());

}

void RenderSystem::v_Shutdown()
{
	m_Plane.Shutdown();
}


}