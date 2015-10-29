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
	windowInfo.title += "ch2-01-Window";
}

void RenderSystem::v_Init()
{
	m_Window.Init();
}

void RenderSystem::v_Render()
{
	//Every frame we get the currentTime
	static float time = 0.0;
	time = static_cast<GLfloat>( glfwGetTime() );

	//Use the current time to change the background color
	const GLfloat bgColor[] =
	{
		sinf(time) * 0.5f + 0.5f,
		cosf(time) * 0.5f + 0.5f,
		0.0f, 1.0f
	};

	//Clear the framework : Color Buffer(index 0) to a random color
	glClearBufferfv(GL_COLOR, 0, bgColor);

	m_Window.Render();

}

void RenderSystem::v_Shutdown()
{
	m_Window.Shutdown();
}
}