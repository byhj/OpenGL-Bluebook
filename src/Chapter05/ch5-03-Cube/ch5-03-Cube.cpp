#include <GL//glew.h>
#include <sb6/sb6.h>
#include <sb6/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CubeApp: public byhj::Application
{
public:
	CubeApp():program(0), vbo(0), vao(0), ibo(0), 
		      CubeShader("Cube Shader") {};
	~CubeApp() {};

	void init_buffer();
	void init_vertexArray();
	void init_shader();

	void v_Init()
	{
		glEnable(GL_DEPTH_TEST);
		init_shader();
		init_buffer();
		init_vertexArray();
	}
	void v_Render()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		static const GLfloat one[] = {1.0f};
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, one);

		//Set the Model View, Proj Matrix
        double time = glfwGetTime();
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), float(time), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0) );
		glm::mat4 proj = glm::perspective(45.0f, static_cast<float>(windowInfo.Width) 
			                           / static_cast<float>(windowInfo.Height), 1.0f, 1000.0f);

		glm::mat4 mvp = proj * view * model;

		glUseProgram(program);
		glBindVertexArray(vao);

		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void v_Shutdown()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
	}
	void v_InitWindowInfo()
	{
		windowInfo.title = "OpenGL-Cube";
	}

private:
	GLuint program, vbo, vao, ibo;
	GLuint mvp_loc;
	Shader CubeShader;
};


CALL_MAIN(CubeApp);

static const GLfloat VertexData[] = 
{  
	//Front
	-0.5f, -0.5f,  0.5f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f,
	-0.5f,  0.5f,  0.5f, 1.0f,
				   
	//Back		   
	-0.5f, -0.5f, -0.5f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f,
	-0.5f,  0.5f, -0.5f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

static const GLuint IndexData[] = 
{
	//Front
	0, 1, 2,
	2, 3, 0,

	//Left
	0, 3, 7,
	7, 4, 0,

	//back
	4, 7, 6,
	6, 5, 4,

	//right
	5, 6, 2,
	2, 1, 5,

	//top
	3, 2, 6,
	6, 7, 3,

	//bottom
	5, 1, 0,
	0, 4, 5,
};
static const GLsizei IndexSize = sizeof(IndexData);

void CubeApp::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CubeApp::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//We change the vao status
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);   
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CubeApp::init_shader()
{
	CubeShader.init();
	CubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	CubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	CubeShader.link();
	program = CubeShader.GetProgram();
	mvp_loc = glGetUniformLocation(program, "mvp");
}