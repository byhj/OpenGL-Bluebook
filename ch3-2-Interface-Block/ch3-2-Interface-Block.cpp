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
	}

	virtual void render()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(program);
		GLfloat time = static_cast<GLfloat>(glfwGetTime());

		GLfloat attrib[] = { sin(time) * 0.5f,  cos(time) * 0.6f, 0.0f, 0.0f };
		glVertexAttrib4fv(0, attrib);    //直接传入shader
	
		const GLfloat color[] = { sin(time) * 0.5f + 0.5f, cos(time) * 0.5f + 0.5f, 0.0f, 1.0f };
		glVertexAttrib4fv(1, color);     //直接传入shader

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


void Triangle::init_shader()
{
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.program;
}

void Triangle::init_buffer()
{
}

void Triangle::init_vertexArray()
{

}