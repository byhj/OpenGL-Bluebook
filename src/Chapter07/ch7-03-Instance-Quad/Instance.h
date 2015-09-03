#ifndef INSTANCE_H
#define INSTANCE_H


#include "ogl/Utility.h"
#include "ogl/Shader.h"

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

	GLuint vao = ogl::VALUE;
	GLuint vbo = ogl::VALUE;
	GLuint program = ogl::VALUE;

	ogl::Shader InstanceShader ={ "Instance Shader" };

};

}
#endif