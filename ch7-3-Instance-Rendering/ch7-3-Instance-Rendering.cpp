#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const GLfloat VertexData[] = {
	-1.0f, -1.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 1.0f
};

static const GLfloat VertexColor[] = {
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f
};

static const GLfloat Position[] = {
	-2.0f, -2.0f, 0.0f, 0.0f,
	 2.0f, -2.0f, 0.0f, 0.0f,
	 2.0f,  2.0f, 0.0f, 0.0f,
	-2.0f,  2.0f, 0.0f, 0.0f
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
	GLuint program, vao, vbo, mvp_loc;
	Shader CubeShader;
};



void IndexCube::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) + sizeof(VertexColor) 
		         + sizeof(Position), NULL, GL_STATIC_DRAW);
	GLuint offset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(VertexData), VertexData);
	offset += sizeof(VertexData);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(VertexColor), VertexColor);
	offset += sizeof(VertexColor);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(Position), Position);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IndexCube::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(VertexData));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(VertexData) + sizeof(VertexColor)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(1, 1);  //每个实例使用不同位置和颜色
	glVertexAttribDivisor(2, 1);
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

	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 4);
}

void IndexCube::init_shader()
{
	CubeShader.attach(GL_VERTEX_SHADER, "quad.vert");
	CubeShader.attach(GL_FRAGMENT_SHADER, "quad.frag");
	CubeShader.link();
	program = CubeShader.program;
}

DECLARE_MAIN(IndexCube);