#include "Cube.h"
#include <glfw/glfw3.h>
#include "ogl/oglDebug.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace byhj
{

Cube::Cube()
{

}

Cube::~Cube()
{

}

void Cube::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Cube::Render(GLfloat aspect)
{

	glUseProgram(program);
	glBindVertexArray(VAO);

	if (sample_shading) {
		glEnable(GL_SAMPLE_SHADING);
		glMinSampleShading(1.0f);
	}
	else
		glDisable(GL_SAMPLE_SHADING);

	static float currentTime;
	currentTime = glfwGetTime() / 100.0f;
#ifdef MANY_CUBES
	for (int i = 0; i < 24; i++) {
		float time = (float)i + (float)currentTime * 0.3f;
		glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -6.0f));
		glm::translate(glm::mat4(1.0), glm::vec3(sinf(2.1f * time)*0.5f, cosf(1.7f*time)*0.5f,
			sinf(1.3f * time) *cosf(1.5f * time) * 2.0f));
		mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 21.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		mv_matrix *= glm::translate(glm::mat4(1.0), glm::vec3(sinf(2.1f * time) * 2.0f, cosf(1.7f * time) * 2.0f,
			sinf(1.3f * time) * cosf(1.5f * time) * 2.0f));
		cubeShader.setUniformMatrix4fv("mv_matrix", 1, 0, glm::value_ptr(mv_matrix));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}
#else
	glm::mat4 proj_matrix = glm::perspective(50.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	float time = (float)currentTime * 0.3f;

	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -4.0f));
	mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(mvp_matrix_loc, 1, 0, glm::value_ptr(proj_matrix * mv_matrix));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
#endif  

	glBindVertexArray(0);
	glUseProgram(0);
}


void Cube::Shutdown()
{
}
static const GLushort ElementData[] =
{
	0, 1, 2,
	2, 1, 3,
	2, 3, 4,
	4, 3, 5,
	4, 5, 6,
	6, 5, 7,
	6, 7, 0,
	0, 7, 1,
	6, 0, 2,
	2, 4, 6,
	7, 5, 3,
	7, 3, 1
};

static const GLfloat VertexData[] =
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

void Cube::init_buffer()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

void Cube::init_vertexArray()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Cube::init_shader()
{
	cubeShader.init();
	cubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	cubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	cubeShader.link();
	cubeShader.use();
	program = cubeShader.GetProgram();
	mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");

}


}