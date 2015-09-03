#ifndef WINDOW_H
#define WINDOW_H

#include "ogl/Utility.h"
#include "ogl/Shader.h"

namespace byhj
{

class Window
{
public:
	Window();
	~Window();

	void Init();
	void Render();
	void Shutdown();

private:
	void init_shader();

	GLuint program = ogl::VALUE;
	GLuint sub_loc = ogl::VALUE;
	GLuint subroutines[2];
	ogl::Shader subShader ={ "Sub Shader" };
};

}
#endif
