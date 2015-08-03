#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/object.h"

namespace byhj
{

class Dragon
{
public:
	Dragon();
	~Dragon();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint program, mvp_loc, mv_loc;
	Shader EquirectangularShader;
	GLuint tex_envmap;
	GLuint envmaps[3];
	int    envmap_index = 0;
	sb6::Object      object;

};

}
#endif
