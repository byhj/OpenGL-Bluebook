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
	init_fbo();
}

void Cube::Render(GLfloat aspect, GLuint sw, GLuint sh)
{

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	static const GLfloat green[] ={ 0.0f, 0.1f, 0.0f, 1.0f };
	static const GLfloat blue[] ={ 0.0f, 0.0f, 0.3f, 1.0f };
	static const GLfloat one = 1.0f;

	static float currentTime;
	currentTime  = glfwGetTime() / 100.0f;
	float time = (float)currentTime * 0.3f;

	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -2.0f))
		* glm::translate(glm::mat4(1.0), glm::vec3(sinf(2.1f * time)*0.5f, cosf(1.7f*time)*0.5f,
			sinf(1.3f * time) *cosf(1.5f * time) * 2.0f));

	mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);

	//We render fbo first, load the framebuffer file as a texture 
	FboShader.use();
	glBindVertexArray(vao);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo); //选择绘制的fbo
	glViewport(0, 0, 512, 512);
	glClearBufferfv(GL_COLOR, 0, green);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glUniformMatrix4fv(fbo_mv_loc, 1, 0, glm::value_ptr(mv_matrix));
	glUniformMatrix4fv(fbo_proj_loc, 1, 0, glm::value_ptr(proj_matrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);  

	glBindVertexArray(0);
	FboShader.end();
										//use fbo texture to render the base scene
	BaseShader.use();
	glBindVertexArray(vao);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, sw, sh);
	glClearBufferfv(GL_COLOR, 0, blue);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glUniform1i(base_prog, 0);

	glUniformMatrix4fv(base_proj_loc, 1, 0, glm::value_ptr(proj_matrix));
	glUniformMatrix4fv(base_mv_loc, 1, 0, glm::value_ptr(mv_matrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	BaseShader.end();
}


void Cube::Shutdown()
{

	glDeleteVertexArrays(1, &vao);

}

///////////////////Vertex Data////////////////////////////////
static const GLushort vertex_indices[] =
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

static const GLfloat vertex_data[] =
{
	// Position                 Tex Coord
	-0.25f, -0.25f, 0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
	0.25f, -0.25f, -0.25f, 1.0f, 0.0f,

	0.25f, -0.25f, -0.25f, 1.0f, 0.0f,
	0.25f, -0.25f, 0.25f, 1.0f, 1.0f,
	-0.25f, -0.25f, 0.25f, 0.0f, 1.0f,

	0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
	0.25f, 0.25f, -0.25f, 1.0f, 0.0f,
	0.25f, -0.25f, 0.25f, 0.0f, 1.0f,

	0.25f, 0.25f, -0.25f, 1.0f, 0.0f,
	0.25f, 0.25f, 0.25f, 1.0f, 1.0f,
	0.25f, -0.25f, 0.25f, 0.0f, 1.0f,

	0.25f, 0.25f, -0.25f, 1.0f, 0.0f,
	-0.25f, 0.25f, -0.25f, 0.0f, 0.0f,
	0.25f, 0.25f, 0.25f, 1.0f, 1.0f,

	-0.25f, 0.25f, -0.25f, 0.0f, 0.0f,
	-0.25f, 0.25f, 0.25f, 0.0f, 1.0f,
	0.25f, 0.25f, 0.25f, 1.0f, 1.0f,

	-0.25f, 0.25f, -0.25f, 1.0f, 0.0f,
	-0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
	-0.25f, 0.25f, 0.25f, 1.0f, 1.0f,

	-0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
	-0.25f, -0.25f, 0.25f, 0.0f, 1.0f,
	-0.25f, 0.25f, 0.25f, 1.0f, 1.0f,

	-0.25f, 0.25f, -0.25f, 0.0f, 1.0f,
	0.25f, 0.25f, -0.25f, 1.0f, 1.0f,
	0.25f, -0.25f, -0.25f, 1.0f, 0.0f,

	0.25f, -0.25f, -0.25f, 1.0f, 0.0f,
	-0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
	-0.25f, 0.25f, -0.25f, 0.0f, 1.0f,

	-0.25f, -0.25f, 0.25f, 0.0f, 0.0f,
	0.25f, -0.25f, 0.25f, 1.0f, 0.0f,
	0.25f, 0.25f, 0.25f, 1.0f, 1.0f,

	0.25f, 0.25f, 0.25f, 1.0f, 1.0f,
	-0.25f, 0.25f, 0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.25f, 0.0f, 0.0f,
};
///////////////////////////////////////////////////////////////

void Cube::init_buffer()
{
	glGenBuffers(1, &position_buffer);  //向shader传入数据
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

	glBindVertexArray(0);
}

void Cube::init_shader()
{
	//fbo
	FboShader.init();
	FboShader.attach(GL_VERTEX_SHADER,   "fbo.vert");
	FboShader.attach(GL_FRAGMENT_SHADER, "fbo.frag");
	FboShader.link();
	fbo_prog= FboShader.GetProgram();
	fbo_mv_loc   = glGetUniformLocation(fbo_prog, "mv_matrix");
	fbo_proj_loc = glGetUniformLocation(fbo_prog, "proj_matrix");


	BaseShader.init();
	BaseShader.attach(GL_VERTEX_SHADER, "cube.vert");
	BaseShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	BaseShader.link();
	base_prog = BaseShader.GetProgram();
	base_mv_loc = glGetUniformLocation(base_prog, "mv_matrix");
	base_proj_loc = glGetUniformLocation(base_prog, "proj_matrix");

}

void Cube::init_fbo()
{
	//Create the framebuffer, render to texture
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//Create color buffer  to save the fbo data
	glGenTextures(1, &color_texture);
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Create depth buffer for fbo
	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexStorage2D(GL_TEXTURE_2D, 9, GL_DEPTH_COMPONENT32F, 512, 512);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

	static const GLenum draw_buffers[] ={ GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}