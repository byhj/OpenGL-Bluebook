#include "Dragon.h"
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ogl/ktx.h"
#include "ogl/oglDebug.h"

namespace byhj
{

Dragon::Dragon()
{

}

Dragon::~Dragon()
{

}

void Dragon::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Dragon::Render(GLfloat aspect)
{

	static double last_time = 0.0;
	static double total_time = 0.0;
	float t = glfwGetTime() / 10;
	glUseProgram(program);

	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f))
		* glm::rotate(glm::mat4(1.0f), (float)t * 1.1f, glm::vec3(0.0, 1.0, 0.0))
		* glm::rotate(glm::mat4(1.0f), (float)t, glm::vec3(1.0, 0.0, 0.0))
		* glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);

	object.render();

	glUseProgram(0);
}


void Dragon::Shutdown()
{
}


void Dragon::init_buffer()
{
	object.load("../../../media/objects/dragon.sbm");

}

void Dragon::init_vertexArray()
{

}

void Dragon::init_shader()
{
	EnvironmentShader.init();
	EnvironmentShader.attach(GL_VERTEX_SHADER, "environment.vert");
	EnvironmentShader.attach(GL_FRAGMENT_SHADER, "environment.frag");
	EnvironmentShader.link();
	program = EnvironmentShader.GetProgram();

	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");

}

void Dragon::init_texture() 
{
	envmaps[0] = sb6::ktx::load("../../../media/textures/envmaps/Dragonmap1.ktx");
	envmaps[1] = sb6::ktx::load("../../../media/textures/envmaps/Dragonmap2.ktx");
	envmaps[2] = sb6::ktx::load("../../../media/textures/envmaps/Dragonmap3.ktx");
	tex_envmap = envmaps[envmap_index];
}


}