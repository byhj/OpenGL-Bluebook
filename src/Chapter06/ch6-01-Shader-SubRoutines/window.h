#ifndef WINDOW_H
#define WINDOW_H

#include "ogl/oglUtility.h"
#include "ogl/oglShader.h"

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

	GLuint program = byhj::OGL_VALUE;
	GLuint sub_loc = byhj::OGL_VALUE;
	GLuint subroutines[2];
	byhj::Shader subShader ={ "Sub Shader" };
};

}
#endif
