#ifndef WINDOW_H
#define WINDOW_H

#include <gl/glew.h>
#include "ogl/oglshader.h"

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
	void ChangeProgram()
	{
		++program_index;
	}
private:
	void init_shader();

	GLuint  program[3], vao;
	GLuint  program_index = 0;
	byhj::Shader  TessellationShader[3];
};

}
#endif
