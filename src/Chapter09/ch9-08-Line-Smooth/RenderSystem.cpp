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
	windowInfo.title += "Line Smooth";
}

void RenderSystem::v_Init()
{

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