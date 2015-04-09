#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>

class Point :public sb6::Application
{
public:
	Point():PointShader("Point shader "), program(0), vao(0), vbo(0){}
	virtual ~Point(){}
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
		glPointSize(40.0f);
		glDrawArrays(GL_POINTS, 0, 1);
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
	Shader PointShader;
};

DECLARE_MAIN(Point);

static const GLfloat VertexData[] = {
	 0.0f, 0.0f, 0.0f
};

void Point::init_shader()
{
	PointShader.attach(GL_VERTEX_SHADER, "point.vert");
	PointShader.attach(GL_FRAGMENT_SHADER, "point.frag");
	PointShader.link();
	program = PointShader.program;
}

void Point::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Point::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}