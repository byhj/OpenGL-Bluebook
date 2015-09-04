#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"

#define NUM_ELEMENTS 2048

namespace byhj
{

class Triangle
{
public:
	Triangle();
	~Triangle();

	void Init();
	void Render();
	void Shutdown();


private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void prefix_sum(const float * input, float * output, int elements);

	GLuint  data_buffer[2];
	float input_data[NUM_ELEMENTS];
	float output_data[NUM_ELEMENTS];
	ogl::Shader PrefixsumShader ={ "Prefixsum Shader" };
	GLuint  prefix_sum_prog;
	GLuint vao;
};

}
#endif
