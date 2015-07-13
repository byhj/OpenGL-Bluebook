#include <GL/glew.h>
#include "ogl/oglApp.h"
#include <ogl/shader.h>


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
{   //Position                 
	-0.5f, -0.5f, 0.0f, 1.0f,  
	0.5f, -0.5f, 0.0f, 1.0f,  
	0.0f,  0.5f, 0.0f, 1.0f,  
};


static const GLfloat ColorData[] =
{
  //Color
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
};

static const GLsizei VertexSize = sizeof(VertexData);
static const GLsizei ColorSize = sizeof(ColorData);

void MapBufferApp::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize + ColorSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, VertexSize, VertexData);
	glBufferSubData(GL_ARRAY_BUFFER, VertexSize, ColorSize, ColorData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MapBufferApp::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);   //stribe: next vertex  || offset: the position of vertex 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(NULL + VertexSize) );

	//Your shout bind the vertexarray to 0 first, bind buffer to 0 will effect it
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