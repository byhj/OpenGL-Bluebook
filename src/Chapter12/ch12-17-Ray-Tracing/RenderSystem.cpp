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
	windowInfo.title += "Sphere";
}

void RenderSystem::v_Init()
{

	m_Sphere.Init( );
}

void RenderSystem::v_Render()
{

	m_Sphere.Render(GetAspect());

}

void RenderSystem::v_Shutdown()
{
	m_Sphere.Shutdown();
}
}