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
		GLfloat time = static_cast<GLfloat>(glfwGetTime());
		const GLfloat color[] = { sin(time) * 0.5f + 0.5f,
			cos(time) * 0.5f + 0.5f,
			0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

	virtual void shutdown()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}
private:
	GLuint program, vao, vbo;
	Shader TriangleShader;
};

DECLARE_MAIN(Triangle);

static const GLfloat VertexData[] = {
	0.25f, -0.25f, 0.5f,
	-0.25f, -0.25f, 0.5f,
	0.25f, 0.25f, 0.5f
};

void Triangle::init_shader()
{
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.program;
}

void Triangle::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}