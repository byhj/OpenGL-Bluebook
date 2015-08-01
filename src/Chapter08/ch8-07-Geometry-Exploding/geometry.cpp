#include "Geometry.h"
#include <glfw/glfw3.h>
#include "ogl/oglDebug.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace byhj
{

Geometry::Geometry()
{

}

Geometry::~Geometry()
{

}

void Geometry::Init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
}

void Geometry::Render(GLfloat aspect)
{

	glUseProgram(program);
	float f = glfwGetTime() / 100.0f;

	glm::mat4 proj_matrix = glm::perspective(50.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -3.0f));
	mv_matrix *= glm::rotate(glm::mat4(1.0), (float)f * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mv_matrix *= glm::rotate(glm::mat4(1.0), (float)f * 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;

	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp_matrix));
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_matrix));
	glUniform1f(explode_factor_loc, sinf((float)f * 8.0f) * cosf((float)f* 6.0f) * 0.7f + 0.1f);

	object.render();

	glUseProgram(0);
}

void Geometry::Shutdown()
{
	glDeleteProgram(program);
}



void Geometry::init_buffer()
{
	object.load("../../../media/objects/bunny_1k.sbm");
}

void Geometry::init_vertexArray()
{


}


void Geometry::init_shader()
{
	CullingShader.init();
	CullingShader.attach(GL_VERTEX_SHADER, "culling.vert");
	CullingShader.attach(GL_GEOMETRY_SHADER, "culling.geom");
	CullingShader.attach(GL_FRAGMENT_SHADER, "culling.frag");
	CullingShader.link();
	program = CullingShader.GetProgram();

	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	explode_factor_loc = glGetUniformLocation(program, "explode_factor");
}

}