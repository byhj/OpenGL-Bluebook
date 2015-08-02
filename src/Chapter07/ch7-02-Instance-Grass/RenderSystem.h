#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ogl/oglApp.h"
#include "Grass.h"

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

private:

	byhj::Grass m_Grass;
};

}

#endif