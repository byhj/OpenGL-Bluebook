#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <ktx.cpp>
#include <object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>

class Gloss : public sb6::Application
{
public:
	Gloss(): program(0){};

	void init();
	void init_shader();
	void render();
	void init_buffer();
	void init_texture();
	void keyboard(GLFWwindow * window, int key, int scancode, int action, int mode);
private:
	GLuint program, mvp_loc, mv_loc;
	Shader GlossShader;
	GLuint tex_envmap;
	GLuint envmap, tex_glossmap;
	Object      object;
};

DECLARE_MAIN(Gloss);

void Gloss::init_shader()
{
	GlossShader.attach(GL_VERTEX_SHADER, "Gloss.vert");
	GlossShader.attach(GL_FRAGMENT_SHADER, "Gloss.frag");
	GlossShader.link();
	program = GlossShader.program;
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

void Gloss::init()
{
	init_shader();
	init_texture();
	object.load("../media/objects/torus_nrms_tc.sbm");
	glEnable(GL_DEPTH_TEST);
}

void Gloss::init_texture()
{
	glActiveTexture(GL_TEXTURE0);
	tex_envmap = ktx::file::load("../media/textures/envmaps/mountaincube.ktx");

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE1);
	tex_glossmap = ktx::file::load("../media/textures/pattern1.ktx");
}

void Gloss::render()
{
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	static const GLfloat ones[] = { 1.0f };

	glClearBufferfv(GL_COLOR, 0, gray);
	glClearBufferfv(GL_DEPTH, 0, ones);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, tex_envmap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_glossmap);

	float t = glfwGetTime() / 1000.0;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))
		                * glm::rotate(glm::mat4(1.0f), (float)t * 13.75f, glm::vec3(0.0, 1.0, 0.0) )
						* glm::rotate(glm::mat4(1.0f), (float)t * 87.75f, glm::vec3(0.0, 0.0, 1.0) )
						* glm::rotate(glm::mat4(1.0f), (float)t * 15.3f, glm::vec3(1.0, 0.0, 0.0) );
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);

	glUseProgram(program);
	object.render();
}

void Gloss::keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

