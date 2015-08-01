#include "Triangle.h"
#include <glfw/glfw3.h>

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

	glUseProgram(program);
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
	glUseProgram(0);
}


void Triangle::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

///////////////////Vertex Data////////////////////////////////

static const GLfloat VertexData[] =
{
	-0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 1.0f,
	 0.0f,  0.5f, 0.0f, 1.0f
};

static const GLsizei VertexSize = sizeof(VertexData);

///////////////////////////////////////////////////////////////

void Triangle::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, NULL, GL_STATIC_DRAW);

	//We map the opengl memory and set the data, avoid the copy the data
	void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	memcpy(ptr, VertexData, VertexSize);

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
}

}