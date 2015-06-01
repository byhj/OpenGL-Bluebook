#include <GL/glew.h>
#include <sb6/shader.h>
#include <sb6/sb6.h>

Shader shader("test");

const static GLfloat VertexData[] = 
{
	-0.5f, -0.5f,
	 0.5f, -0.5f,
	 0.0f,  0.5f
};

class test_app : public sb6::Application 
{
public:
	void init_shader()
	{
		shader.attach(GL_VERTEX_SHADER, "triangle.vert");
		shader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
		shader.link();
		shader.use();
		shader.interfaceInfo();
		program = shader.GetProgram();
	}

	void init_buffer()
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void init_vertexArray()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);   //bind the vbo to vao, send the data to shader
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);
	}
	void vInitWindowInfo()
	{
		windowInfo.title = "Test";
		windowInfo.Width = 1200;
		windowInfo.Height = 800;
	}

	void vInit()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
	}

	void vRender()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);

		glBindVertexArray(vao);
		glUseProgram(program);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	virtual void vShutdown()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}

private:
	GLuint  program;
	GLuint  vbo, vao, ibo;;
};

DECLARE_MAIN(test_app);