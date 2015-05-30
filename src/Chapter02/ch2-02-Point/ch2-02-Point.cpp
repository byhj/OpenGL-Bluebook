#include <GL/glew.h>
#include <sb6/sb6.h>
#include <sb6/shader.h>

class PointApp : public sb6::Application
{
public:
	PointApp():PointShader("Point Shader") {}
	virtual ~PointApp() {};

	void init_shader();
	void init_buffer();
	void init_vertexArray();

	void init()
	{
		//init the data and status
		init_shader();
		init_buffer();
		init_vertexArray();
	}
	
	void render()
	{
		//Use this shader and vao data to render
		glUseProgram(program);
		glBindVertexArray(vao);

		//Render Point, which is 40 pixel 
		glPointSize(40.0f);
		glDrawArrays(GL_POINTS, 0, 1);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	//We shutdown the object
	void shutdown()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

private:
  GLuint program, vao, vbo;
  Shader PointShader;

};

DECLARE_MAIN(PointApp);

static const GLfloat VertexData[] = {0.0f, 0.0f, 0.0f, 1.0f};
static const GLsizei VertexCount  = sizeof(VertexData);

void PointApp::init_shader()
{
	PointShader.init();
	PointShader.attach(GL_VERTEX_SHADER, "point.vert");
	PointShader.attach(GL_FRAGMENT_SHADER, "point.frag");
	PointShader.link();
	program = PointShader.GetProgram();
	glUseProgram(program);
}

void PointApp::init_buffer()
{
	//Sent the Vertex Data to vbo, which is a memoery manage by opengl
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexCount, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointApp::init_vertexArray()
{
	//We use vao to manage the opengl status, include vbo, ibo, tbo etc...
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Enable vertexAttribArray , sent the vbo vertex data to shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//We should always disable the status after use it
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}