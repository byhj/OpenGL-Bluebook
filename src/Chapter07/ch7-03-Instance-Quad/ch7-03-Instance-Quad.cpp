#include <GL/glew.h>
#include "ogl/oglApp.h"
#include "ogl/shader.h"

class InstanceQuadApp:  public byhj::Application
{
public:
	InstanceQuadApp()
		:program(0), vao(0), vbo(0),
		InstanceQuadShader("InstanceQuad")
	{
	}
	~InstanceQuadApp(){}

	void init_buffer();
	void init_vertexArray();
	void init_shader();

	void v_Init()
	{
		init_buffer();
		init_vertexArray();
		init_shader();
	}
	void v_Render()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(program);
		glBindVertexArray(vao);

         //We draw four instance quad with different pos and color
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 4);

		glBindVertexArray(0);
		glUseProgram(0);
	}

private:
	GLuint program, vao, vbo;
	Shader InstanceQuadShader;
};

CALL_MAIN(InstanceQuadApp);

static const GLfloat VertexData[] = 
{
  -0.5f, -0.5f, -0.0f, 1.0f,
   0.5f, -0.5f, -0.0f, 1.0f,
   0.5f,  0.5f, -0.0f, 1.0f,
  -0.5f,  0.5f, -0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

static const GLfloat ColorData[] =
{
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
};
static const GLsizei ColorSize = sizeof(ColorData);


static const GLfloat OffsetData[] = 
{
	-1.0f, -1.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,
};
static const GLsizei OffsetSize = sizeof(OffsetData);

void InstanceQuadApp::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize + ColorSize + OffsetSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, VertexSize, VertexData);
	glBufferSubData(GL_ARRAY_BUFFER, VertexSize, ColorSize, ColorData);
	glBufferSubData(GL_ARRAY_BUFFER, VertexSize + ColorSize, OffsetSize, OffsetData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
};

void InstanceQuadApp::init_vertexArray()
{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)VertexSize);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(VertexSize + ColorSize) );

		//We set the index 1&2 send to shader each instance not each position
		//You can use gl_InstanceID to achieve the current instance index
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstanceQuadApp::init_shader()
{
   InstanceQuadShader.init();
   InstanceQuadShader.attach(GL_VERTEX_SHADER, "instance.vert");
   InstanceQuadShader.attach(GL_FRAGMENT_SHADER, "instance.frag");
   InstanceQuadShader.link();
   InstanceQuadShader.interfaceInfo();
   program = InstanceQuadShader.GetProgram();
}