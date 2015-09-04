#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/object.h"

namespace byhj
{

class Cube
{
public:
	Cube();
	~Cube();

	void Init(int sw, int sh);
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_fbo();

	GLuint width, height;
	GLuint program;
	ogl::Shader LayerShader, ShowShader;
	sb6::Object object;
	GLuint vao, transform_ubo, array_texture, array_depth, layered_fbo;
	GLuint program_layer, program_show;
};

}
#endif
