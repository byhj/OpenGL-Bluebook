#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/object.h"

namespace byhj
{

class Plane
{
public:
	Plane();
	~Plane();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint program;
	GLuint mv_loc, mvp_loc, vp_loc;
	ogl::Shader ClampingShader;
	sb6::Object object;
	bool depthClamp;
};

}
#endif
