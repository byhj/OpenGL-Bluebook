#include "Instance.h"

namespace byhj
{

void Instance::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Instance::Render()
{
	glUseProgram(program);
	glBindVertexArray(vao);

	//We draw four instance quad with different pos and color
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 4);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Instance::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

static const GLfloat VertexData[] =
{
	-0.5f, -0.5f, -0.0f, 1.0f,
	0.5f, -0.5f, -0.0f, 1.0f,
	0.5f,  0.5f, -0.0f, 1.0f,
	-0.5f,  0.5f, -0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

static const GLfloat ColorData[] =
{
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
};
static const GLsizei ColorSize = sizeof(ColorData);


static const GLfloat OffsetData[] =
{
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f,
	1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,
};
static const GLsizei OffsetSize = sizeof(OffsetData);

void Instance::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize + ColorSize + OffsetSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, VertexSize, VertexData);
	glBufferSubData(GL_ARRAY_BUFFER, VertexSize, ColorSize, ColorData);
	glBufferSubData(GL_ARRAY_BUFFER, VertexSize + ColorSize, OffsetSize, OffsetData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Instance::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)VertexSize);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(VertexSize + ColorSize));

	//We set the index 1&2 send to shader each instance not each position
	//You can use gl_InstanceID to achieve the current instance index
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Instance::init_shader()
{
	InstanceShader.init();
	InstanceShader.attach(GL_VERTEX_SHADER, "instance.vert");
	InstanceShader.attach(GL_FRAGMENT_SHADER, "instance.frag");
	InstanceShader.link();
	InstanceShader.info();
	program = InstanceShader.GetProgram();
}



}