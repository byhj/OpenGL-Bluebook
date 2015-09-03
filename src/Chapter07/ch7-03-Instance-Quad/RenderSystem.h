#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ogl/App.h"
#include "Instance.h"

namespace byhj
{

class RenderSystem : public ogl::App
{
public:
	RenderSystem();
	~RenderSystem();

public:
	void v_InitInfo();
	void v_Init();
	void v_Render();
	void v_Shutdown();

private:

	byhj::Instance m_Instance;
};

}

#endif