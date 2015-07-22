#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ogl/oglApp.h"
#include "Texture.h"

namespace byhj
{

class RenderSystem : public byhj::Application
{
public:
	RenderSystem();
	~RenderSystem();

public:
	void v_InitInfo();
	void v_Init();
	void v_Render();
	void v_Shutdown();

	void v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
private:

	byhj::Texture m_Texture;
};

}

#endif