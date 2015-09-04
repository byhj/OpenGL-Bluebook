#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/object.h"

namespace byhj
{

class Sphere
{
public:
	Sphere();
	~Sphere();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	GLuint program, mvp_loc, mv_loc;
	ogl::Shader NormalMappingShader;
	GLuint lightPos_loc;
	sb6::Object object;
	GLuint tex_color, tex_normal;

};

}
#endif
