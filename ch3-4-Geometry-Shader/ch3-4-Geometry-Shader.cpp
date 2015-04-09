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
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void render()
	{
	   static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
       glClearBufferfv(GL_COLOR, 0, green);

	   glUseProgram(program);
	   glPointSize(10.0f);
	   glDrawArrays(GL_PATCHES, 0, 3); //细分要用这个绘制
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
	TriangleShader.attach(GL_TESS_CONTROL_SHADER, "triangle.tcs");
    TriangleShader.attach(GL_TESS_EVALUATION_SHADER, "triangle.tes");
	TriangleShader.attach(GL_GEOMETRY_SHADER, "triangle.geom");
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