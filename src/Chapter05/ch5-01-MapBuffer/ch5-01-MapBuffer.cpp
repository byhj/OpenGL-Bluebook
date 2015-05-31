#include <GL/glew.h>
#include <sb6/sb6.h>
#include <sb6/shader.h>


class MapBufferApp: public sb6::Application
{
public:
	MapBufferApp():MapBufferShader("MapBufferShader") {};
	~MapBufferApp(){};

	void init_buffer();
	void init_vertexArray();
	void init_shader();

	void vInit()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
	}
	void vRender()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	void vShutdown()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
	void vInitWindowInfo()
	{
		windowInfo.title = "OpenGL-MapBuffer";
	}

private:
	GLuint program, vao, vbo;
	Shader MapBufferShader;
};

DECLARE_MAIN(MapBufferApp);

static const GLfloat VertexData[] = 
{
	-0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 1.0f,
	 0.0f,  0.5f, 0.0f, 1.0f
};

static const GLsizei VertexSize = sizeof(VertexData);

void MapBufferApp::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, NULL, GL_STATIC_DRAW);

	void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, VertexData, VertexSize);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MapBufferApp::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MapBufferApp::init_shader()
{
	MapBufferShader.init();
	MapBufferShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	MapBufferShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	MapBufferShader.link();
	program = MapBufferShader.GetProgram();
}