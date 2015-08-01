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

	static float f = glfwGetTime();
	f += 0.01;

	glUseProgram(program);
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_matrix));

	//different viewpoint
	GLfloat vViewpoint[] ={ sinf(f * 2.1f) * 70.0f, cosf(f * 1.4f) * 70.0f, sinf(f * 0.7f) * 70.0f };
	glUniform3fv(vp_loc, 1, vViewpoint);
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
	vp_loc = glGetUniformLocation(program, "viewpoint");
}

}