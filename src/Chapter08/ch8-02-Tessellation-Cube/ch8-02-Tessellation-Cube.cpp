#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6/sb6.h>
#include <sb6/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class IndexCube: public byhj::Application
{
public:
	IndexCube(){};
	~IndexCube(){}

	void v_Init();
	void v_Render();

	void init_shader();
	void init_buffer();
	void init_vertexArray();
public:
	GLuint  program, vao;
	GLuint  position_buffer , index_buffer;
	GLint   mv_matrix_loc, proj_matrix_loc;
	Shader  cubeShader;
};

static const GLushort vertex_indices[] =
{
	0, 1, 2, 3,
	2, 3, 4, 5,
	4, 5, 6, 7,
	6, 7, 0, 1,
	0, 2, 6, 4,
	1, 7, 3, 5
};

static const GLfloat vertex_positions[] =
{
	-0.25f, -0.25f, -0.25f,
	-0.25f,  0.25f, -0.25f,
	0.25f, -0.25f, -0.25f,
	0.25f,  0.25f, -0.25f,
	0.25f, -0.25f,  0.25f,
	0.25f,  0.25f,  0.25f,
	-0.25f, -0.25f,  0.25f,
	-0.25f,  0.25f,  0.25f,
};

void IndexCube::init_buffer()
{
	glGenBuffers(1, &position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexCube::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

}

void IndexCube::v_Init()
{
	init_shader();
	init_buffer();
	init_vertexArray();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void IndexCube::v_Render()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_COLOR, 0, green);
	glClearBufferfv(GL_DEPTH, 0, &one);

	cubeShader.use();
	glBindVertexArray(vao);
	glm::mat4 proj_matrix = glm::perspective(50.0f, GetAspect(), 0.1f, 1000.0f);
	glUniformMatrix4fv(proj_matrix_loc, 1, 0, &proj_matrix[0][0]);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	static float currentTime ;
	currentTime = glfwGetTime();
	for (int i = 0; i < 100; i++) 
	{
		float time = (float)i + (float)currentTime * 0.03f;
		glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -10.0f)) *
			                  glm::translate(glm::mat4(1.0), glm::vec3(sinf(2.1f * time), cosf(1.7f*time),
			                                                           sinf(4.3f * time) *cosf(3.5f * time) * 10.0f) );

		mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 3.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 5.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(mv_matrix_loc, 1, 0,  &mv_matrix[0][0]);
		glDrawElements(GL_PATCHES, 24, GL_UNSIGNED_SHORT, 0);
	}
}

void IndexCube::init_shader()
{
	cubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	cubeShader.attach(GL_TESS_CONTROL_SHADER, "cube.tcs");
	cubeShader.attach(GL_TESS_EVALUATION_SHADER, "cube.tes");
	cubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	cubeShader.link();
	cubeShader.use();
	program = cubeShader.GetProgram();
	mv_matrix_loc = glGetUniformLocation(program, "mv_matrix");
	proj_matrix_loc = glGetUniformLocation(program, "proj_matrix");
}

CALL_MAIN(IndexCube);



