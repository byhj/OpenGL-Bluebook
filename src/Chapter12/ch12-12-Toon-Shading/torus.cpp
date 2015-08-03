#include "Torus.h"
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ogl/ktx.h"
#include "ogl/oglDebug.h"

namespace byhj
{

Torus::Torus()
{

}

Torus::~Torus()
{

}

void Torus::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Torus::Render(GLfloat aspect)
{

	glUseProgram(program);


	glBindTexture(GL_TEXTURE_1D, tex_toon);

	float t = glfwGetTime() / 100.0;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))
		* glm::rotate(glm::mat4(1.0f), (float)t * 13.75f, glm::vec3(0.0, 1.0, 0.0))
		* glm::rotate(glm::mat4(1.0f), (float)t * 87.75f, glm::vec3(0.0, 0.0, 1.0))
		* glm::rotate(glm::mat4(1.0f), (float)t * 15.3f, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);

	object.render();

	glUseProgram(0);
}


void Torus::Shutdown()
{
}


void Torus::init_buffer()
{
	object.load("../../../media/objects/torus_nrms_tc.sbm");

}

void Torus::init_vertexArray()
{

}

void Torus::init_shader()
{
	ToonShader.init();
	ToonShader.attach(GL_VERTEX_SHADER, "Toon.vert");
	ToonShader.attach(GL_FRAGMENT_SHADER, "Toon.frag");
	ToonShader.link();
	program = ToonShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");

}

static const GLubyte toon_tex_data[] =
{
	0x44, 0x00, 0x00, 0x00,
	0x88, 0x00, 0x00, 0x00,
	0xCC, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0x00
};

void Torus::init_texture() 
{

	glGenTextures(1, &tex_toon);
	glBindTexture(GL_TEXTURE_1D, tex_toon);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB8, sizeof(toon_tex_data) / 4);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, sizeof(toon_tex_data) / 4,
		            GL_RGBA, GL_UNSIGNED_BYTE, toon_tex_data);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
}


}