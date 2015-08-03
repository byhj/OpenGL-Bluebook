#include "Plane.h"
#include "ogl/ktx.h"
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace byhj
{

Plane::Plane()
{

}

Plane::~Plane()
{

}

void Plane::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Plane::Render(GLfloat aspect)
{

	float t = (float)glfwGetTime() / 100.0f;

	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) *
		glm::rotate(glm::mat4(1.0f), (float)t * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::rotate(glm::mat4(1.0f), (float)t * 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 proj_matrix = glm::perspective(50.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mvp = proj_matrix * mv_matrix;

	glUseProgram(prog_noise);
	glUniform1f(loc_time, t * 0.0002);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(0);
}


void Plane::Shutdown()
{
	glDeleteProgram(prog_noise);
	glDeleteVertexArrays(1, &vao);
}

void Plane::init_buffer()
{

}

void Plane::init_vertexArray()
{

}

void Plane::init_shader()
{
	NoiseShader.init();
	NoiseShader.attach(GL_VERTEX_SHADER, "noise.vert");
	NoiseShader.attach(GL_FRAGMENT_SHADER, "noise.frag");
	NoiseShader.link();
	prog_noise = NoiseShader.GetProgram();
	loc_time = glGetUniformLocation(prog_noise, "time");
}

void Plane::init_texture()
{

}

}