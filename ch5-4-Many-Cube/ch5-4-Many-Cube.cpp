#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define  MANY_CUBES

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
		glEnable(GL_DEPTH_TEST);
		init_shader();
		init_buffer();
		init_vertexArray();
	}

	virtual void render()
	{
		static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);
		static const GLfloat one[] = { 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);
		glUseProgram(program);

#ifdef MANY_CUBES
		float time = (float)glfwGetTime();
		for (int i = 0; i != 24; ++i) 
		{
			float f = static_cast<float>(i) + time / 3;
			glm::mat4 proj = glm::perspective(50.0f, static_cast<float>(windowInfo.Width) 
				/ static_cast<float>(windowInfo.Height),
				0.1f, 1000.0f);
			glm::mat4 mv = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f))
				         * glm::translate(glm::mat4(1.0f), glm::vec3( sinf(6.1f * f), cosf(5.7f * f),
				                          sinf(1.3f * f) * cosf(1.5f * f) * 2.0f) )
				         * glm::rotate(glm::mat4(1.0f), glm::radians(time * 45.0f), glm::vec3(0.0f, 1.0f, 0.0f))
				         * glm::rotate(glm::mat4(1.0f), glm::radians(time * 81.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 mvp = proj * mv;
			glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
			glBindVertexArray(0);
		}
#else
		float time = (float)glfwGetTime();
		glm::mat4 proj = glm::perspective(50.0f, static_cast<float>(windowInfo.Width) 
			/ static_cast<float>(windowInfo.Height),
			0.1f, 1000.0f);
		glm::mat4 mv = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f))
			* glm::translate(glm::mat4(1.0f), glm::vec3( sinf(2.1f * time) * 0.5f, cosf(1.7f * time) * 0.5f,
			sinf(1.3f * time) * cosf(1.5f * time) * 2.0f) )
			* glm::rotate(glm::mat4(1.0f), glm::radians(time * 45.0f), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(time * 81.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 mvp = proj * mv;
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
#endif
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}
private:
	GLuint program, vao, vbo, ibo;
	Shader TriangleShader;
	GLuint mvp_loc;
};

DECLARE_MAIN(Triangle);


void Triangle::init_shader()
{
	TriangleShader.attach(GL_VERTEX_SHADER, "cube.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "cube.frag");

	TriangleShader.link();
	program = TriangleShader.program;
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

const static GLfloat VertexData[] = {
	// front
	-0.25, -0.25,  0.25,
	0.25, -0.25,  0.25,
	0.25,  0.25,  0.25,
	-0.25,  0.25,  0.25,
	// back
	-0.25, -0.25, -0.25,
	0.25, -0.25, -0.25,
	0.25,  0.25, -0.25,
	-0.25,  0.25, -0.25,
};
const static GLushort ElementData[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// top
	3, 2, 6,
	6, 7, 3,
	// back
	7, 6, 5,
	5, 4, 7,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// left
	4, 0, 3,
	3, 7, 4,
	// right
	1, 5, 6,
	6, 2, 1,
};

void Triangle::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Triangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,  NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);   //让vao知道使用这个ibo
	glBindVertexArray(0);
}