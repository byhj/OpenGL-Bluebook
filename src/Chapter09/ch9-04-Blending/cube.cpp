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

void Cube::Render(GLfloat aspect, GLuint sw, GLuint sh)
{

	glUseProgram(program);
	glBindVertexArray(vao);

	static const GLenum blend_func[] =  //19»ìºÏÄ£Ê½
	{
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA,
		GL_ONE_MINUS_CONSTANT_ALPHA,
		GL_SRC_ALPHA_SATURATE,
		GL_SRC1_COLOR,
		GL_ONE_MINUS_SRC1_COLOR,
		GL_SRC1_ALPHA,
		GL_ONE_MINUS_SRC1_ALPHA
	};

	static const int num_blend_funcs = sizeof(blend_func) / sizeof(blend_func[0]);
	static const float x_scale = 20.0f / float(num_blend_funcs);
	static const float y_scale = 16.0f / float(num_blend_funcs);

	glUseProgram(program);
	glBindVertexArray(vao);

	glEnable(GL_BLEND);
	glBlendColor(0.2f, 0.5f, 0.7f, 0.5f);

	glm::mat4 proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);

	float currentTime = glfwGetTime() / 100.0f;
	for (int j = 0; j < num_blend_funcs; j++)
	{
		for (int i = 0; i < num_blend_funcs; i++)
		{
			float time = currentTime;
			glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(9.5f - x_scale * float(i),
				7.5f - y_scale * float(j), -70.0f));
			//We set the cube position with offset
			mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * -45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * -21.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(mvp_loc, 1, 0, glm::value_ptr(proj_matrix * mv_matrix));

			//Call the different blend function
			glBlendFunc(blend_func[i], blend_func[j]);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		}
	}

	glBindVertexArray(0);
	glUseProgram(0);
}


void Cube::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);

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
	-0.25f, -0.25f, -0.25f,
	-0.25f,  0.25f, -0.25f,
	0.25f, -0.25f, -0.25f,
	0.25f,  0.25f, -0.25f,
	0.25f, -0.25f,  0.25f,
	0.25f,  0.25f,  0.25f,
	-0.25f, -0.25f,  0.25f,
	-0.25f,  0.25f,  0.25f,
};
///////////////////////////////////////////////////////////////

void Cube::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Cube::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");

}

}