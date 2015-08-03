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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, tex_envmap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_glossmap);

	float t = glfwGetTime() / 1000.0;
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
	GlossShader.init();
	GlossShader.attach(GL_VERTEX_SHADER, "Gloss.vert");
	GlossShader.attach(GL_FRAGMENT_SHADER, "Gloss.frag");
	GlossShader.link();
	program = GlossShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");

}

void Torus::init_texture() 
{
	glActiveTexture(GL_TEXTURE0);
	tex_envmap = sb6::ktx::load("../../../media/textures/envmaps/mountaincube.ktx");

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE1);
	tex_glossmap = sb6::ktx::load("../../../media/textures/pattern1.ktx");
}


}