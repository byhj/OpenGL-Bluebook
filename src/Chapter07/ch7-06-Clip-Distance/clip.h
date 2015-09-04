#ifndef Clip_H
#define Clip_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/object.h"

namespace byhj
{

class Clip
{
public:
	Clip();
	~Clip();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	ogl::Shader ClipShader{ "Clip Shader" };
	GLuint program = 0;
	sb6::Object object;

	GLint   proj_loc;
	GLint   mv_loc;
	GLint   clip_plane_loc;
	GLint   clip_sphere_loc;
};

}
#endif
