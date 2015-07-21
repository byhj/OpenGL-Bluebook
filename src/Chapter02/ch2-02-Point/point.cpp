#include "Point.h"

namespace byhj
{

Point::Point()
{

}

Point::~Point()
{

}

void Point::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Point::Render()
{
	//Use this shader and vao data to render
	glUseProgram(program);
	glBindVertexArray(vao);

	//Render Point, which is 40 pixel 
	glPointSize(40.0f);
	glDrawArrays(GL_POINTS, 0, 1);

	glBindVertexArray(0);
	glUseProgram(0);
}


void Point::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

static const GLfloat VertexData[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static const GLsizei VertexCount = sizeof(VertexData);

void Point::init_buffer()
{
	//Sent the Vertex Data to vbo, which is a memory manage by opengl
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexCount, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Point::init_vertexArray()
{
	//We use vao to manage the opengl status, include vbo, ibo, tbo etc...
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Enable vertexAttribArray , sent the vbo vertex data to shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//We should always disable the status after use it
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Point::init_shader()
{
	PointShader.init();
	PointShader.attach(GL_VERTEX_SHADER, "point.vert");
	PointShader.attach(GL_FRAGMENT_SHADER, "point.frag");
	PointShader.link();
	program = PointShader.GetProgram();
}


}