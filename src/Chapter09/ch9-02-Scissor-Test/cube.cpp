#include "Cube.h"
#include <glfw/glfw3.h>
#include "ogl/oglDebug.h"

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

void Cube::Render(GLfloat aspect, GLuint sw, GLuint sh)
{


	static const int width = sw;
	static const int height = sh;

	static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	static const GLfloat one[] ={ 1.0f };
	glClearBufferfv(GL_DEPTH, 0, one);

	cubeShader.use();
	glBindVertexArray(VAO);

	glEnable(GL_SCISSOR_TEST);

	int scissor_width = (7 * width) / 16; // 7/16window
	int scissor_height = (7 * height) / 16;
	// Lower left...
	glScissorIndexed(0,     //index
		0, 0, //left bottom
		scissor_width, scissor_height); // width height
										// Lower right...
	glScissorIndexed(1,
		width - scissor_width, 0,
		width - scissor_width, scissor_height);
	//Upper left...
	glScissorIndexed(2,
		0, height - scissor_height,
		scissor_width, scissor_height);
	// Upper right...
	glScissorIndexed(3,
		width - scissor_width, height - scissor_height,
		scissor_width, scissor_height);

	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	static float currentTime;
	currentTime = glfwGetTime() / 100.0f;
	float f = (float)currentTime * 0.3f;

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO); //binding  = 0
	glm::mat4 * mv_matrix_array = (glm::mat4 *) glMapBufferRange(
		GL_UNIFORM_BUFFER,
		0, 4 * sizeof(glm::mat4),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
		);
	for (int i = 0; i < 4; i++)
	{
		mv_matrix_array[i] = proj_matrix
			* glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -2.0f))
			* glm::rotate(glm::mat4(1.0), (float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0), (float)currentTime * 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_SCISSOR_TEST);
}


void Cube::Shutdown()
{


}

///////////////////Vertex Data////////////////////////////////
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
	-0.75f, -0.75f, -0.75f,
	-0.75f,  0.75f, -0.75f,
	0.75f, -0.75f, -0.75f,
	0.75f,  0.75f, -0.75f,
	0.75f, -0.75f,  0.75f,
	0.75f,  0.75f,  0.75f,
	-0.75f, -0.75f,  0.75f,
	-0.75f,  0.75f,  0.75f,
};

///////////////////////////////////////////////////////////////

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

	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void Cube::init_vertexArray()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Cube::init_shader()
{
	cubeShader.init();
	cubeShader.attach(GL_VERTEX_SHADER, "scissor.vert");
	cubeShader.attach(GL_GEOMETRY_SHADER, "scissor.geom");
	cubeShader.attach(GL_FRAGMENT_SHADER, "scissor.frag");
	cubeShader.link();

}

}