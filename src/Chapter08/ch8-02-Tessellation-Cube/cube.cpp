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
	init_shader();
	init_buffer();
	init_vertexArray();
}

void Cube::Render(GLfloat aspect)
{

	CubeShader.use();
	glBindVertexArray(vao);


	glm::mat4 proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);
	glUniformMatrix4fv(proj_matrix_loc, 1, 0, &proj_matrix[0][0]);

	static float currentTime;

	currentTime = glfwGetTime();
	for (int i = 0; i < 100; i++)
	{
		float time = (float)i + (float)currentTime * 0.03f;
		glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -10.0f)) *
			glm::translate(glm::mat4(1.0), glm::vec3(sinf(2.1f * time), cosf(1.7f*time),
				sinf(4.3f * time) *cosf(3.5f * time) * 10.0f));

		mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 3.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 5.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(mv_matrix_loc, 1, 0, &mv_matrix[0][0]);

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_PATCHES, 24, GL_UNSIGNED_SHORT, 0);
	}

	CubeShader.end();

}

void Cube::Shutdown()
{
	glDeleteProgram(program);
}


static const GLushort vertex_indices[] =
{
	0, 1, 2, 3,
	2, 3, 4, 5,
	4, 5, 6, 7,
	6, 7, 0, 1,
	0, 2, 6, 4,
	1, 7, 3, 5
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

void Cube::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

}


void Cube::init_shader()
{
	CubeShader.init();
	CubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	CubeShader.attach(GL_TESS_CONTROL_SHADER, "cube.tcs");
	CubeShader.attach(GL_TESS_EVALUATION_SHADER, "cube.tes");
	CubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	CubeShader.link();
	program = CubeShader.GetProgram();

	mv_matrix_loc = glGetUniformLocation(program, "mv_matrix");
	proj_matrix_loc = glGetUniformLocation(program, "proj_matrix");
}

}