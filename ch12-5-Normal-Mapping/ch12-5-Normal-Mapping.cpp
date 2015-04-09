#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <ktx.cpp>
#include <object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>

class NormalMapping : public sb6::Application
{
public:
	NormalMapping(): program(0){}
	void init();
	void init_shader();
	void render();
	void init_buffer();
	void init_texture();
	void keyboard(GLFWwindow * window, int key, int scancode, int action, int mode);
private:
	GLuint program, mvp_loc, mv_loc;
	Shader NormalMappingShader;
	GLuint lightPos_loc;
	Object object;
	GLuint tex_color, tex_normal;
};

DECLARE_MAIN(NormalMapping);

void NormalMapping::init_shader()
{
	NormalMappingShader.attach(GL_VERTEX_SHADER, "normal.vert");
	NormalMappingShader.attach(GL_FRAGMENT_SHADER, "normal.frag");
	NormalMappingShader.link();
	program = NormalMappingShader.program;
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	lightPos_loc = glGetUniformLocation(program, "lightPos");
}

void NormalMapping::init()
{
	init_shader();
	init_texture();
	object.load("../media/objects/ladybug.sbm");
	glEnable(GL_DEPTH_TEST);
}

void NormalMapping::init_texture()
{
	glActiveTexture(GL_TEXTURE0);
	tex_color = ktx::file::load("../media/textures/ladybug_co.ktx");
	glActiveTexture(GL_TEXTURE1);
	tex_normal = ktx::file::load("../media/textures/ladybug_nm.ktx");
}
void NormalMapping::render()
{
	static const GLfloat zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	static const GLfloat ones[] = { 1.0f };
	glClearBufferfv(GL_COLOR, 0, gray);
	glClearBufferfv(GL_DEPTH, 0, ones);


	float t = glfwGetTime();
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, -5.5f))
		                * glm::rotate(glm::mat4(1.0f), 14.5f, glm::vec3(1.0, 0.0, 0.0) )
						* glm::rotate(glm::mat4(1.0f), -20.0f, glm::vec3(0.0, 1.0, 0.0) );
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	glUniform3fv(lightPos_loc, 1, &glm::vec3(40.0f * sinf(t), 30.0f + 20.0f * cosf(t), 40.0f)[0]);
	glUseProgram(program);
	object.render();
}

void NormalMapping::keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

