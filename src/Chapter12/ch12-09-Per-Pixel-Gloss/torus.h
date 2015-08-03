#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/object.h"

namespace byhj
{

class Torus
{
public:
	Torus();
	~Torus();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint program, mvp_loc, mv_loc;
	Shader GlossShader;
	GLuint tex_envmap;
	GLuint envmap, tex_glossmap;
	sb6::Object      object;

};

}
#endif
