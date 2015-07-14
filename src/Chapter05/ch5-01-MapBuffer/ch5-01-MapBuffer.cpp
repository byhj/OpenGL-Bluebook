#include <GL/glew.h>
#include "ogl/oglApp.h"
#include "ogl/shader.h"


class MapBufferApp: public byhj::Application
{
public:
	MapBufferApp():MapBufferShader("MapBufferShader") {};
	~MapBufferApp(){};

	void init_buffer();
	void init_vertexArray();
	void init_shader();

	void v_Init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
	}
	void v_Render()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	void v_Shutdown()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
	void v_InitWindowInfo()
	{
		windowInfo.title = "OpenGL-MapBuffer";
	}

private:
	GLuint program, vao, vbo;
	Shader MapBufferShader;
};

CALL_MAIN(MapBufferApp);

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

	//We map the opengl memotry and set the data, avoid the copy the data
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