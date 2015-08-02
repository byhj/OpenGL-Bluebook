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

	float t = glfwGetTime();

	glUseProgram(program);
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.5f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(sin(t) * 60.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	sphereObject.render();

	glUseProgram(0);
}


void Sphere::Shutdown()
{
}


void Sphere::init_buffer()
{
	sphereObject.load("../../../media/objects/sphere.sbm");


}

void Sphere::init_vertexArray()
{

}

void Sphere::init_shader()
{
	BlinnShader.init();
	BlinnShader.attach(GL_VERTEX_SHADER,   "blinn.vert");
	BlinnShader.attach(GL_FRAGMENT_SHADER, "blinn.frag");
	BlinnShader.link();
	program = BlinnShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");

}


}