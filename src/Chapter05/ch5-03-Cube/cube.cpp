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
	glEnable(GL_DEPTH_TEST);

	init_buffer();
	init_vertexArray();
	init_shader();
}

void Cube::Render(const ogl::MvpMatrix &matrix)
{

	glUseProgram(program);
	glBindVertexArray(vao);

	glm::mat4 model = matrix.model;
	glm::mat4 view  = matrix.view;
	glm::mat4 proj  = matrix.proj;
	glm::mat4 mvp = proj * view * model;
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}


void Cube::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

///////////////////Vertex Data////////////////////////////////

static const GLfloat VertexData[] =
{
	//Front
	-0.5f, -0.5f,  0.5f, 1.0f,
	0.5f, -0.5f,  0.5f, 1.0f,
	0.5f,  0.5f,  0.5f, 1.0f,
	-0.5f,  0.5f,  0.5f, 1.0f,

	//Back		   
	-0.5f, -0.5f, -0.5f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f,
	0.5f,  0.5f, -0.5f, 1.0f,
	-0.5f,  0.5f, -0.5f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

static const GLuint IndexData[] =
{
	//Front
	0, 1, 2,
	2, 3, 0,

	//Left
	0, 3, 7,
	7, 4, 0,

	//back
	4, 7, 6,
	6, 5, 4,

	//right
	5, 6, 2,
	2, 1, 5,

	//top
	3, 2, 6,
	6, 7, 3,

	//bottom
	5, 1, 0,
	0, 4, 5,
};
static const GLsizei IndexSize = sizeof(IndexData);

///////////////////////////////////////////////////////////////

void Cube::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::init_shader()
{
	CubeShader.init();
	CubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	CubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	CubeShader.link();
	program = CubeShader.GetProgram();
	mvp_loc = glGetUniformLocation(program, "mvp");

	//If false, throw a exception
	assert(mvp_loc != ogl::VALUE);
}

}