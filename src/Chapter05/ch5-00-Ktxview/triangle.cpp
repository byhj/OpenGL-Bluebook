#include "Triangle.h"
#include "ogl/ktx.h"

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
	init_texture();
}

void Triangle::Render()
{
	glUseProgram(program);

	float t = static_cast<float>( glfwGetTime() );
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1f(exposure_loc, (float)(sin(t) * 16.0 + 16.0));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

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
	KtxShader.init();
	KtxShader.attach(GL_VERTEX_SHADER, "ktx.vert");
	KtxShader.attach(GL_FRAGMENT_SHADER, "ktx.frag");
	KtxShader.link();
	program = KtxShader.GetProgram();
	exposure_loc = glGetUniformLocation(program, "exposure");
}

void Triangle::init_texture()
{
	glGenTextures(1, &texture);
	sb6::ktx::load("../../../media/textures/tree.ktx", texture);
	glBindTexture(GL_TEXTURE_2D, 0);
}

}