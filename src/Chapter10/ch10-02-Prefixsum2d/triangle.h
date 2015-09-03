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

	GLuint images[3];
	GLuint  prefix_sum_prog;
	GLuint  show_image_prog;
	GLuint  dummy_vao;
	Shader Prefixsum2dShader;
	Shader ShowShader;
	GLuint vao;
};

}
#endif
