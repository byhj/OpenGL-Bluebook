#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <ktx.cpp>
#include <object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>

class CubeMapping : public sb6::Application
{
public:
	CubeMapping()
		: envmap_index(0){};

	void init();
	void init_shader();
	void render();
	void init_buffer();
	void init_texture();
	void keyboard(GLFWwindow * window, int key, int scancode, int action, int mode);
private:
	GLuint skybox_prog, render_prog, mvp_loc, mv_loc, view_loc, skybox_vao;
	Shader renderShader, skyboxShader;
	GLuint tex_envmap;
	GLuint envmaps[3];
	int    envmap_index;
	Object object;
};

DECLARE_MAIN(CubeMapping);

void CubeMapping::init_shader()
{
	renderShader.attach(GL_VERTEX_SHADER, "render.vert");
	renderShader.attach(GL_FRAGMENT_SHADER, "render.frag");
	renderShader.link();
	render_prog = renderShader.program;
	mv_loc = glGetUniformLocation(render_prog, "mv_matrix");
	mvp_loc = glGetUniformLocation(render_prog, "mvp_matrix");

	skyboxShader.attach(GL_VERTEX_SHADER, "skybox.vert");
	skyboxShader.attach(GL_FRAGMENT_SHADER, "skybox.frag");
	skyboxShader.link();
	skybox_prog = skyboxShader.program;
	view_loc = glGetUniformLocation(skybox_prog, "view_matrix");
}

void CubeMapping::init()
{        
	glGenVertexArrays(1, &skybox_vao);
    glBindVertexArray(skybox_vao);
	init_shader();
	init_texture();
	object.load("../media/objects/dragon.sbm");
	glEnable(GL_DEPTH_TEST);
}

void CubeMapping::init_texture()
{
	envmaps[0] = ktx::file::load("../media/textures/envmaps/mountaincube.ktx");
	tex_envmap = envmaps[envmap_index];
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void CubeMapping::render()
{
	static const GLfloat gray[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	static const GLfloat ones[] = { 1.0f };
	glClearBufferfv(GL_COLOR, 0, gray);
	glClearBufferfv(GL_DEPTH, 0, ones);

	float t = glfwGetTime() / 1000.0;
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(15.0f * sinf(t), 0.0f, 15.0f * cosf(t)),
		                                glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mv_matrix = view_matrix
		* glm::rotate(glm::mat4(1.0f), (float)t, glm::vec3(1.0, 0.0, 0.0) )
		* glm::rotate(glm::mat4(1.0f), (float)t * 130.1f, glm::vec3(0.0, 1.0, 0.0) )
		* glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
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
}

void CubeMapping::keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

