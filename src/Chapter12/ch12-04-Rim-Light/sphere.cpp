#include "Sphere.h"
#include <glfw/glfw3.h>
#include "ogl/oglDebug.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace byhj
{

Sphere::Sphere()
{

}

Sphere::~Sphere()
{

}

void Sphere::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Sphere::Render(GLfloat aspect)
{

	glUseProgram(program);

	static const GLfloat green[] ={ 0.0f, 0.25f, 0.0f, 1.0f };
	static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat one = 1.0f;
	static double last_time = 0.0;
	static double total_time = 0.0;

	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glUniform3fv(rim_color_loc, 1, &(rim_enable ? rim_color : glm::vec3(0.0f))[0]);
	glUniform1f(rim_power_loc, rim_power);

	float t = glfwGetTime() / 1000.0;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, -20.0f))
		* glm::rotate(glm::mat4(1.0f), sin(t) * 60.0f, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);

	glUseProgram(program);
	object.render();

	glUseProgram(0);
}


void Sphere::Shutdown()
{
}


void Sphere::init_buffer()
{
	object.load("../../../media/objects/dragon.sbm");


}

void Sphere::init_vertexArray()
{

}

void Sphere::init_shader()
{
	RimLightShader.init();
	RimLightShader.attach(GL_VERTEX_SHADER, "rim.vert");
	RimLightShader.attach(GL_FRAGMENT_SHADER, "rim.frag");
	RimLightShader.link();
	program = RimLightShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	rim_color_loc = glGetUniformLocation(program, "rim_color");
	rim_power_loc = glGetUniformLocation(program, "rim_power");

}


}