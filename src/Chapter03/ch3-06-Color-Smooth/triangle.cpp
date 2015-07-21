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

	//We use current time change the vertex position every frame,
	//it will animate just a cricle .
	GLfloat time = static_cast<GLfloat>(glfwGetTime());
	GLfloat offset[] = {
		(float)sin(time) * 0.5f,
		(float)cos(time) * 0.5f,
		0.0f, 0.0f
	};

	glVertexAttrib4fv(0, offset);
	glUniform1i(glGetUniformLocation(program, "isFlat"), flat);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(0);
}


void Triangle::Shutdown()
{
	glDeleteProgram(program);
}

void Triangle::ChangeFlat()
{
	flat = !flat;
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