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
	m_Window.Init();
}

void RenderSystem::v_Render()
{
	//Every frame we get the currentTime
	static double time = 0.0;
	time = glfwGetTime();

	//Use the current time to change the backgroud color
	const GLfloat bgColor[] =
	{
		static_cast<float>(sin(time)) * 0.5f + 0.5f,
		static_cast<float>(cos(time)) * 0.5f + 0.5f,
		0.0f, 1.0f
	};

	//Clear the color buffer to the random color
	glClearBufferfv(GL_COLOR, 0, bgColor);

	m_Window.Render();

}

void RenderSystem::v_Shutdown()
{
	m_Window.Shutdown();
}
}