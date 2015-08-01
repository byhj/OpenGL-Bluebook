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

	static float currentTime;
	currentTime = glfwGetTime();
	float f = (float)currentTime;

	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -5.0f));
	mv_matrix *= glm::rotate(glm::mat4(1.0), glm::radians((float)currentTime * 71.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mv_matrix *= glm::rotate(glm::mat4(1.0), glm::radians((float)currentTime * 10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;

	glUniformMatrix4fv(mvp_matrix_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	glUniformMatrix4fv(mv_matrix_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniform1f(stretch_loc, sinf(f * 4.0f) * 0.75f + 1.0f);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, NULL);

	glBindVertexArray(0);

	glUseProgram(0);
}

void Geometry::Shutdown()
{
	glDeleteProgram(program);
}


static const GLfloat tetrahedron_verts[] ={
	0.000f,  0.000f,  1.000f,
	0.943f,  0.000f, -0.333f,
	-0.471f,  0.816f, -0.333f,
	-0.471f, -0.816f, -0.333f
};

static const GLushort tetrahedron_indices[] ={
	0, 1, 2,
	0, 2, 3,
	0, 3, 1,
	3, 2, 1
};

void Geometry::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedron_verts), tetrahedron_verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetrahedron_indices), tetrahedron_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Geometry::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

}


void Geometry::init_shader()
{
	cubeShader.init();
	cubeShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	cubeShader.attach(GL_GEOMETRY_SHADER, "triangle.geom");
	cubeShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	cubeShader.link();
	cubeShader.use();
	program =  cubeShader.GetProgram();

	mv_matrix_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");
	stretch_loc = glGetUniformLocation(program, "stretch");
}

}