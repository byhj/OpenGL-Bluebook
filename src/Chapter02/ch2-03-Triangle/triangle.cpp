#include "Triangle.h"

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
	//Use this shader and vao data to render
	glUseProgram(program);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(0);
}


void Triangle::Shutdown()
{
	glDeleteProgram(program);
}

void Triangle::init_buffer()
{

}

void Triangle::init_vertexArray()
{

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