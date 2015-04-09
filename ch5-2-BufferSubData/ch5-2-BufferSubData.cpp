#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>

class Triangle :public sb6::Application
{
public:
	Triangle():TriangleShader("Triangle shader"), program(0), vao(0), vbo(0){}
	~Triangle(){}
    void init_shader();
	void init_buffer();
	void init_vertexArray();
	virtual void init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
	}

	virtual void render()
	{
	   static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
       glClearBufferfv(GL_COLOR, 0, green);

	   glUseProgram(program);
	   glBindVertexArray(vao);
	   glDrawArrays(GL_TRIANGLES, 0, 3);
	   glBindVertexArray(0);
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}
private:
	GLuint program, vao, vbo;
	Shader TriangleShader;
};

DECLARE_MAIN(Triangle);


void Triangle::init_shader()
{
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");

	TriangleShader.link();
	program = TriangleShader.program;
}

static const GLfloat data[] = {
	0.25f, -0.25f, 0.5f, 1.0f,
	-0.25f, -0.25f, 0.5f, 1.0f,
	0.25f, 0.25f, 0.5f, 1.0f
};

static const GLfloat color[] = {
	1.0, 0.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 0.0, 1.0, 1.0
};

void Triangle::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data) + sizeof(color), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(data), sizeof(color), color);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); //前一个参数是多少数组成一个点，后面的参数是数据的开始位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(data) + NULL));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}