#include <gl/glew.h>
#include "ogl/oglApp.h"
#include "ogl/ktx.cpp"
#include "ogl/object.cpp"
#include "ogl/shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Environment : public byhj::Application
{
public:
	Environment()
		: program(0), envmap_index(2){};

	void v_Init();
	void v_Render();

	void init_shader();
	void init_buffer();
	void init_texture();
private:
	GLuint program, mvp_loc, mv_loc;
	Shader EnvironmentShader;
	GLuint tex_envmap;
	GLuint envmaps[3];
	int    envmap_index;
	sb6::Object      object;
};

CALL_MAIN(Environment);

void Environment::init_shader()
{
	EnvironmentShader.attach(GL_VERTEX_SHADER, "environment.vert");
	EnvironmentShader.attach(GL_FRAGMENT_SHADER, "environment.frag");
	EnvironmentShader.link();
	program = EnvironmentShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

void Environment::v_Init()
{
	init_shader();
	init_texture();
	object.load("../../../media/objects/dragon.sbm");
	glEnable(GL_DEPTH_TEST);
}

void Environment::init_texture()
{
	envmaps[0] = sb6::ktx::load("../../../media/textures/envmaps/spheremap1.ktx");
	envmaps[1] = sb6::ktx::load("../../../media/textures/envmaps/spheremap2.ktx");
	envmaps[2] = sb6::ktx::load("../../../media/textures/envmaps/spheremap3.ktx");
	tex_envmap = envmaps[envmap_index];
}

void Environment::v_Render()
{
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat one = 1.0f;
	static double last_time = 0.0;
	static double total_time = 0.0;

	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	float t = glfwGetTime() / 10;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f))
		                * glm::rotate(glm::mat4(1.0f), (float)t * 1.1f, glm::vec3(0.0, 1.0, 0.0) )
						* glm::rotate(glm::mat4(1.0f), (float)t, glm::vec3(1.0, 0.0, 0.0) )
						* glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);

	glUseProgram(program);
	object.render();
}

