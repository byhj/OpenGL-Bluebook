#include "Dragon.h"
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ogl/ktx.h"
#include "ogl/oglDebug.h"

namespace byhj
{

Dragon::Dragon()
{

}

Dragon::~Dragon()
{

}

void Dragon::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Dragon::Render(GLfloat aspect)
{

	static double last_time = 0.0;
	static double total_time = 0.0;

	float t = glfwGetTime() / 1000.0;
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(15.0f * sinf(t), 0.0f, 15.0f * cosf(t)),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mv_matrix = view_matrix
		* glm::rotate(glm::mat4(1.0f), (float)t, glm::vec3(1.0, 0.0, 0.0))
		* glm::rotate(glm::mat4(1.0f), (float)t * 130.1f, glm::vec3(0.0, 1.0, 0.0))
		* glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;

	glUseProgram(skybox_prog);
	glBindVertexArray(skybox_vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_envmap);
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view_matrix[0][0]);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(render_prog);
	glEnable(GL_DEPTH_TEST);

	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	object.render();

	glUseProgram(0);
}


void Dragon::Shutdown()
{
}


void Dragon::init_buffer()
{
	object.load("../../../media/objects/dragon.sbm");

}

void Dragon::init_vertexArray()
{

}

void Dragon::init_shader()
{
	renderShader.init();
	renderShader.attach(GL_VERTEX_SHADER, "render.vert");
	renderShader.attach(GL_FRAGMENT_SHADER, "render.frag");
	renderShader.link();
	render_prog = renderShader.GetProgram();
	mv_loc = glGetUniformLocation(render_prog, "mv_matrix");
	mvp_loc = glGetUniformLocation(render_prog, "mvp_matrix");

	skyboxShader.init();
	skyboxShader.attach(GL_VERTEX_SHADER, "skybox.vert");
	skyboxShader.attach(GL_FRAGMENT_SHADER, "skybox.frag");
	skyboxShader.link();
	skybox_prog = skyboxShader.GetProgram();
	view_loc = glGetUniformLocation(skybox_prog, "view_matrix");
}

void Dragon::init_texture() 
{
	envmaps[0] = sb6::ktx::load("../../../media/textures/envmaps/mountaincube.ktx");
	tex_envmap = envmaps[envmap_index];
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}


}