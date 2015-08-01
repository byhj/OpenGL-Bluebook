#ifndef INSTANCE_H
#define INSTANCE_H


#include "ogl/oglUtility.h"
#include "ogl/oglShader.h"

namespace byhj
{


class Instance
{
public:
	Instance() = default;
	~Instance() = default;

	void Init();
	void Render();
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	GLuint vao = byhj::OGL_VALUE;
	GLuint vbo = byhj::OGL_VALUE;
	GLuint program = byhj::OGL_VALUE;

	byhj::Shader InstanceShader ={ "Instance Shader" };

};

}
#endif