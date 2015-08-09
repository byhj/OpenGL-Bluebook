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