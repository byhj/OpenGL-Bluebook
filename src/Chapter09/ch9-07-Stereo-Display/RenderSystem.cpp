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
	windowInfo.title += "Stereo Diaplay";
}

void RenderSystem::v_Init()
{

	glEnable(GL_DEPTH_TEST);
	m_Cube.Init( );
}

void RenderSystem::v_Render()
{

	m_Cube.Render(GetAspect());

}

void RenderSystem::v_Shutdown()
{
	m_Cube.Shutdown();
}
}