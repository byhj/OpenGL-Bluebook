#include "grass.h"

#include "ogl/vmath.h"
#include "ogl/ktx.h"

#include <GLFW/glfw3.h>

namespace byhj
{

void Grass::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}


void Grass::Render(float aspect)
{

	float currentTime = static_cast<float> ( glfwGetTime() );
	float t = (float)currentTime * 0.02f;
	float r = 550.0f;

	vmath::mat4 mv = vmath::lookat(vmath::vec3(sinf(t) * r, 25.0f, cosf(t) * r),
		vmath::vec3(0.0f, -50.0f, 0.0f),
		vmath::vec3(0.0, 1.0, 0.0));
	vmath::mat4 proj = vmath::perspective(45.0f, aspect, 0.1f, 1000.0f);

	glUseProgram(program);
	glBindVertexArray(vao);

	glUniformMatrix4fv(uniformLoc.mvp, 1, GL_FALSE, (proj * mv));
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 1024 * 1024);

	glUseProgram(0);
	glBindVertexArray(0);
}

void Grass::Shutdown()
{
	glDeleteProgram(program);
}

static const GLfloat grass_blade[] =
{
	-0.3f, 0.0f,
	0.3f, 0.0f,
	-0.20f, 1.0f,
	0.1f, 1.3f,
	-0.05f, 2.3f,
	0.0f, 3.3f
};

void Grass::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grass_blade), grass_blade, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Grass::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
}

void Grass::init_shader()
{
	GrassShader.init();
	GrassShader.attach(GL_VERTEX_SHADER, "grass.vert");
	GrassShader.attach(GL_FRAGMENT_SHADER, "grass.frag");
	GrassShader.link();
	GrassShader.info();
	program = GrassShader.GetProgram();

	uniformLoc.mvp = glGetUniformLocation(program, "mvp");
	uniformLoc.tex_grass_length      = glGetUniformLocation(program, "tex_grass_length");
	uniformLoc.tex_grass_orientation = glGetUniformLocation(program, "tex_grass_orientation");
	uniformLoc.tex_grass_color       = glGetUniformLocation(program, "tex_grass_color");
	uniformLoc.tex_grass_bend        = glGetUniformLocation(program, "tex_grass_bend");

	glUseProgram(program);

	glUniform1i(uniformLoc.tex_grass_length, 1);
	glUniform1i(uniformLoc.tex_grass_orientation, 2);
	glUniform1i(uniformLoc.tex_grass_color, 3);
	glUniform1i(uniformLoc.tex_grass_bend, 4);

	glUseProgram(0);
}

void Grass::init_texture()
{
	glActiveTexture(GL_TEXTURE1);
	texID.tex_grass_length = sb6::ktx::load("../../../media/textures/grass_length.ktx");

	glActiveTexture(GL_TEXTURE2);
	texID.tex_grass_orientation = sb6::ktx::load("../../../media/textures/grass_orientation.ktx");

	glActiveTexture(GL_TEXTURE3);
	texID.tex_grass_color = sb6::ktx::load("../../../media/textures/grass_color.ktx");

	glActiveTexture(GL_TEXTURE4);
	texID.tex_grass_bend = sb6::ktx::load("../../../media/textures/grass_bend.ktx");

}

}