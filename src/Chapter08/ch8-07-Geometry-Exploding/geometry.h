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

	GLuint program;
	GLuint mv_loc, mvp_loc, explode_factor_loc;
	Shader CullingShader;
	sb6::Object object;
};

}
#endif
