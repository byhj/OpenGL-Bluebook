#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ogl/App.h"
#include "window.h"

namespace byhj
{

class RenderSystem : public ogl::App
{
public:
	RenderSystem();
	~RenderSystem();

public:
	void v_InitInfo()  override;
	void v_Init()      override;
	void v_Render()    override;
	void v_Shutdown()  override;

private:

	byhj::Window m_Window;
};

}

#endif