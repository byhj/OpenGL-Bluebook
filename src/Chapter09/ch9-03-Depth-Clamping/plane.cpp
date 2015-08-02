#include "Plane.h"
#include <glfw/glfw3.h>
#include "ogl/oglDebug.h"

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
	init_shader();
	init_buffer();
	init_vertexArray();
}



void Plane::Render(GLfloat aspect)
{

	if (depthClamp)
		glEnable(GL_DEPTH_CLAMP);
	else
		glDisable(GL_DEPTH_CLAMP);
	float f = glfwGetTime();
	glUseProgram(program);
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1.0f))
		* glm::rotate(glm::mat4(1.0), glm::radians(f* 45.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0), glm::radians(f* 81.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_matrix));

	object.render();

	glUseProgram(0);


}

void Plane::Shutdown()
{

}


void Plane::init_shader()
{
	ClampingShader.init();
	ClampingShader.attach(GL_VERTEX_SHADER, "depthClamp.vert");
	ClampingShader.attach(GL_FRAGMENT_SHADER, "depthClamp.frag");
	ClampingShader.link();
	program = ClampingShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");

}


void Plane::init_vertexArray()
{

}

void Plane::init_buffer()
{
	object.load("../../../media/objects/bunny_1k.sbm");
}


}