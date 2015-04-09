#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <ktx.cpp>
#include <object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>

class Environment : public sb6::Application
{
public:
	Environment()
		: program(0), envmap_index(2){};

	void init();
	void init_shader();
	void render();
	void init_buffer();
	void init_texture();
	void keyboard(GLFWwindow * window, int key, int scancode, int action, int mode);
private:
	GLuint program, mvp_loc, mv_loc;
	Shader EnvironmentShader;
	GLuint tex_envmap;
	GLuint envmaps[3];
	int    envmap_index;
	Object      object;
};

DECLARE_MAIN(Environment);

void Environment::init_shader()
{
	EnvironmentShader.attach(GL_VERTEX_SHADER, "environment.vert");
	EnvironmentShader.attach(GL_FRAGMENT_SHADER, "environment.frag");
	EnvironmentShader.link();
	program = EnvironmentShader.program;
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

void Environment::init()
{
	init_shader();
	init_texture();
	object.load("../media/objects/dragon.sbm");
	glEnable(GL_DEPTH_TEST);
}

void Environment::init_texture()
{
	envmaps[0] = ktx::file::load("../media/textures/envmaps/spheremap1.ktx");
	envmaps[1] = ktx::file::load("../media/textures/envmaps/spheremap2.ktx");
	envmaps[2] = ktx::file::load("../media/textures/envmaps/spheremap3.ktx");
	tex_envmap = envmaps[envmap_index];
}

void Environment::render()
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

void Environment::keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

