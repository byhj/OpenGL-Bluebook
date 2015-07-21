#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ogl/oglApp.h"
#include "Point.h"

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

	byhj::Point m_Point;
};

}

#endif