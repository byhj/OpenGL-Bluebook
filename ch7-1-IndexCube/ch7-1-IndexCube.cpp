#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const GLfloat VertexData[] = {
	-0.25f, -0.25f, -0.25f,
	-0.25f,  0.25f, -0.25f,
	0.25f, -0.25f, -0.25f,
	0.25f,  0.25f, -0.25f,
	0.25f, -0.25f,  0.25f,
	0.25f,  0.25f,  0.25f,
	-0.25f, -0.25f,  0.25f,
	-0.25f,  0.25f,  0.25f,
};

static const GLushort ElementData[] =
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

class IndexCube: public sb6::Application
{
public:
	IndexCube(){};
	~IndexCube(){}
    virtual void init();
	virtual void render();
	void init_shader();
	void init_buffer();
	void init_vertexArray();
public:
	GLuint program, vao, vbo, ibo, mvp_loc;
	Shader CubeShader;
};



void IndexCube::init_buffer()
{
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glGenBuffers(1, &ibo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexCube::init_vertexArray()
{
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
   glBindVertexArray(vao);
}

void IndexCube::init()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	init_shader();
	init_buffer();
	init_vertexArray();
}

void IndexCube::render()
{
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0, 0.0, 0.0, 0.0)[0]);
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_DEPTH, 0, &one);
	glUseProgram(program);
    glBindVertexArray(vao);
	float f = (float)glfwGetTime();
	glm::mat4 mvp_matrix = glm::rotate(glm::mat4(1.0f), f, glm::vec3(1.0f, 1.0f, 1.0f)) ;
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	glPolygonMode(GL_FRONT, GL_LINE);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

void IndexCube::init_shader()
{
   CubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
   CubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
   CubeShader.link();
   program = CubeShader.program;
   mvp_loc = glGetUniformLocation(program, "mvp");
}

DECLARE_MAIN(IndexCube);