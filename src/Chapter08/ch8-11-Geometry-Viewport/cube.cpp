#include "Cube.h"
#include <glfw/glfw3.h>
#include "ogl/oglDebug.h"

namespace byhj
{

Cube::Cube()
{

}

Cube::~Cube()
{

}

void Cube::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Cube::Render(GLfloat aspect, GLuint sw, GLuint sh)
{

	glUseProgram(program);
	glBindVertexArray(vao);

	int i;

	// Each rectangle will be 7/16 of the screen
	float viewport_width = (float)(7 * sw) / 16.0f;
	float viewport_height = (float)(7 * sh) / 16.0f;

	// Four rectangles - lower left first...
	glViewportIndexedf(0, 0, 0, viewport_width, viewport_height);

	// Lower right...
	glViewportIndexedf(1,
		sw - viewport_width, 0,
		viewport_width, viewport_height);

	// Upper left...
	glViewportIndexedf(2,
		0, sh - viewport_height,
		viewport_width, viewport_height);

	// Upper right...
	glViewportIndexedf(3,
		sw - viewport_width,
		sh - viewport_height,
		viewport_width, viewport_height);

	glm::mat4 proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);

	float f = (float)glfwGetTime() * 0.01f;

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);
	glm::mat4 * mv_matrix_array = (glm::mat4 *)glMapBufferRange(GL_UNIFORM_BUFFER,
		0,
		4 * sizeof(glm::mat4),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (i = 0; i < 4; i++)
	{
		mv_matrix_array[i] = proj_matrix
			* glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f))
			* glm::rotate(glm::mat4(1.0f), (float)f * 45.0f * (float)(i + 1), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), (float)f * 81.0f * (float)(i + 1), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}


void Cube::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);

}

///////////////////Vertex Data////////////////////////////////
static const GLushort vertex_indices[] =
{
	0, 1, 2,
	2, 1, 3,
	2, 3, 4,
	4, 3, 5,
	4, 5, 6,
	6, 5, 7,
	6, 7, 0,
	0, 7, 1,
	6, 0, 2,
	2, 4, 6,
	7, 5, 3,
	7, 3, 1
};

static const GLfloat vertex_positions[] =
{
	-0.25f, -0.25f, -0.25f,
	-0.25f,  0.25f, -0.25f,
	0.25f, -0.25f, -0.25f,
	0.25f,  0.25f, -0.25f,
	0.25f, -0.25f,  0.25f,
	0.25f,  0.25f,  0.25f,
	-0.25f, -0.25f,  0.25f,
	-0.25f,  0.25f,  0.25f,
};

///////////////////////////////////////////////////////////////

void Cube::init_buffer()
{
	glGenBuffers(1, &position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &uniform_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void Cube::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);

	glBindVertexArray(0);
}

void Cube::init_shader()
{
	viewShader.init();
	viewShader.attach(GL_VERTEX_SHADER, "view.vert");
	viewShader.attach(GL_GEOMETRY_SHADER, "view.geom");
	viewShader.attach(GL_FRAGMENT_SHADER, "view.frag");
	viewShader.link();
	program = viewShader.GetProgram();

}

}