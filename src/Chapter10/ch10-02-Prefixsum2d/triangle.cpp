#include "Triangle.h"
#include <GLFW/glfw3.h>
#include "ogl/ktx.h"

static inline float random_float()
{
	static unsigned int seed = 0x13371337;

	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

namespace byhj
{

Triangle::Triangle()
{

}

Triangle::~Triangle()
{

}

void Triangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Triangle::Render()
{

	double currentTime = glfwGetTime();
	glUseProgram(prefix_sum_prog);

	glBindImageTexture(0, images[0], 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	glBindImageTexture(1, images[1], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

	glDispatchCompute(NUM_ELEMENTS, 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindImageTexture(0, images[1], 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	glBindImageTexture(1, images[2], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

	glDispatchCompute(NUM_ELEMENTS, 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindTexture(GL_TEXTURE_2D, images[2]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, images[2]);

	glUseProgram(show_image_prog);
	glBindVertexArray(dummy_vao);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glUseProgram(0);
}


void Triangle::Shutdown()
{

}

void Triangle::init_buffer()
{
	int i;

	glGenTextures(3, images);

	images[0] = sb6::ktx::load("../../../media/textures/floor.ktx");

	for (i = 1; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, images[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, NUM_ELEMENTS, NUM_ELEMENTS);
	}

	glGenVertexArrays(1, &dummy_vao);
	glBindVertexArray(dummy_vao);

}

void Triangle::init_vertexArray()
{

}

void Triangle::init_shader()
{
	Prefixsum2dShader.init();
	Prefixsum2dShader.attach(GL_COMPUTE_SHADER, "prefixsum2d.comp");
	Prefixsum2dShader.link();
	prefix_sum_prog = Prefixsum2dShader.GetProgram();

	ShowShader.init();
	ShowShader.attach(GL_VERTEX_SHADER, "showimage.vert");
	ShowShader.attach(GL_FRAGMENT_SHADER, "showimage.frag");
	ShowShader.link();
	show_image_prog = ShowShader.GetProgram();
}

void Triangle::prefix_sum(const float * input, float * output, int elements)
{
	float f = 0.0f;
	int i;

	for (i = 0; i < elements; i++)
	{
		f += input[i];
		output[i] = f;
	}
}
}