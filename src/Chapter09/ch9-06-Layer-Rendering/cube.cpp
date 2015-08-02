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

void Cube::Init(int sw, int sh)
{
	width  = sw;
	height = sh;
	init_buffer();
	init_vertexArray();
	init_shader();
	init_fbo();
}

void Cube::Render(GLfloat aspect)
{

	static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat gray[] ={ 0.1f, 0.1f, 0.1f, 1.0f };
	static const GLfloat one = 1.0f;
	float t = glfwGetTime() / 100.0f;

	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f))
		* glm::rotate(glm::mat4(1.0f), (float)t * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::rotate(glm::mat4(1.0f), (float)t * 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	glm::mat4 mvp = proj_matrix * mv_matrix;

	struct TRANSFORM_BUFFER
	{
		glm::mat4 proj_matrix;
		glm::mat4 mv_matrix[16];
	};

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, transform_ubo);

	TRANSFORM_BUFFER * buffer = (TRANSFORM_BUFFER *)glMapBufferRange
		(
			GL_UNIFORM_BUFFER, 0, sizeof(TRANSFORM_BUFFER),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
			);

	buffer->proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f); // proj_matrix;
	int i;

	for (i = 0; i < 16; i++)
	{
		float fi = (float)(i + 12) / 16.0f;
		buffer->mv_matrix[i] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f))
			* glm::rotate(glm::mat4(1.0f), (float)t * 25.0f * fi, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::rotate(glm::mat4(1.0f), (float)t * 30.0f * fi, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	static const GLenum ca0 = GL_COLOR_ATTACHMENT0;

	glBindFramebuffer(GL_FRAMEBUFFER, layered_fbo);
	glDrawBuffers(1, &ca0);
	glViewport(0, 0, width, height);
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(program_layer);

	object.render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glUseProgram(program_show);
	glClearBufferfv(GL_COLOR, 0, gray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, array_texture);
	glDisable(GL_DEPTH_TEST);

	glBindVertexArray(vao);
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 16);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}


void Cube::Shutdown()
{
	glDeleteVertexArrays(1, &vao);
}

void Cube::init_buffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	object.load("../../../media/objects/torus.sbm");

	glGenBuffers(1, &transform_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, transform_ubo);
	glBufferData(GL_UNIFORM_BUFFER, 17 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);

}

void Cube::init_vertexArray()
{

}

void Cube::init_shader()
{
	ShowShader.init();
	ShowShader.attach(GL_VERTEX_SHADER, "show.vert");
	ShowShader.attach(GL_FRAGMENT_SHADER, "show.frag");
	ShowShader.link();
	program_show = ShowShader.GetProgram();

	LayerShader.init();
	LayerShader.attach(GL_VERTEX_SHADER, "layer.vert");
	LayerShader.attach(GL_GEOMETRY_SHADER, "layer.geom");
	LayerShader.attach(GL_FRAGMENT_SHADER, "layer.frag");
	LayerShader.link();
	program_layer = LayerShader.GetProgram();

}

void Cube::init_fbo()
{

	glGenTextures(1, &array_texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, array_texture);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, 16);

	glGenTextures(1, &array_depth);
	glBindTexture(GL_TEXTURE_2D_ARRAY, array_depth);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT32, width, height, 16);

	//bind the fbo to color and depth buffer, notice the texture is array for layer render
	glGenFramebuffers(1, &layered_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, layered_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, array_texture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, array_depth, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

}