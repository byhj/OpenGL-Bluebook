#include "Triangle.h"
#include <GLFW/glfw3.h>

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

	glUseProgram(prefix_sum_prog);
	glBindVertexArray(vao);

	float * ptr;
	double currentTime = glfwGetTime();
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, data_buffer[0], 0, sizeof(float) * NUM_ELEMENTS);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * NUM_ELEMENTS, input_data);

	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, data_buffer[1], 0, sizeof(float) * NUM_ELEMENTS);

	glDispatchCompute(1, 1, 1);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glFinish();

	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, data_buffer[1], 0, sizeof(float) * NUM_ELEMENTS);
	ptr = (float *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * NUM_ELEMENTS, GL_MAP_READ_BIT);

	char buffer[1024];
	sprintf(buffer, "%2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f "
		"%2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f\n",
		ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7],
		ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15]);
	//    OutputDebugStringA(buffer);

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	//    Sleep(500);
	glBindVertexArray(0);
	glUseProgram(0);
}


void Triangle::Shutdown()
{

}

void Triangle::init_buffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, data_buffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data_buffer[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_ELEMENTS * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, data_buffer[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_ELEMENTS * sizeof(float), NULL, GL_DYNAMIC_COPY);

	int i;

	for (i = 0; i < NUM_ELEMENTS; i++)
	{
		input_data[i] = random_float();
	}

	prefix_sum(input_data, output_data, NUM_ELEMENTS);

	glBindVertexArray(0);

}

void Triangle::init_vertexArray()
{

}

void Triangle::init_shader()
{
	PrefixsumShader.init();
	PrefixsumShader.attach(GL_COMPUTE_SHADER, "prefixsum.comp");
	PrefixsumShader.link();
	prefix_sum_prog = PrefixsumShader.GetProgram();
	/*
	prefix_sum_prog = glCreateProgram();
	glAttachShader(prefix_sum_prog, cs);

	glLinkProgram(prefix_sum_prog);

	int n;
	glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &n);
	*/

	glShaderStorageBlockBinding(prefix_sum_prog, 0, 0);
	glShaderStorageBlockBinding(prefix_sum_prog, 1, 1);
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