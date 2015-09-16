#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/object.h"

namespace byhj
{

class Geometry
{
public:
	Geometry();
	~Geometry();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	ogl::Shader cubeShader;
	GLuint program, mv_matrix_loc, mvp_matrix_loc;
	GLuint stretch_loc, vao, vbo, ibo;
};

}
#endif
