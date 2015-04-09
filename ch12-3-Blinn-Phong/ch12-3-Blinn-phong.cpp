#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>

class Phong: public sb6::Application
{
public:
	void init();
	void init_shader();
	void render();
	void init_buffer();
private:
	GLuint program, mvp_loc, mv_loc;
	Shader PhongShader;
	Object sphereObject;
};
DECLARE_MAIN(Phong);

void Phong::init_shader()
{
	PhongShader.attach(GL_VERTEX_SHADER, "blinn.vert");
	PhongShader.attach(GL_FRAGMENT_SHADER, "blinn.frag");
	PhongShader.link();
	program = PhongShader.program;
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

void Phong::init()
{
	init_shader();
	sphereObject.load("../media/objects/sphere.sbm");
	glEnable(GL_DEPTH_TEST);
}

void Phong::render()
{
	static const float one[] = {1.0f};
	static const float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, one);

	glUseProgram(program);

	float t = glfwGetTime() / 1000.0;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.5f))
		* glm::rotate(glm::mat4(1.0f), sin(t) * 60.0f, glm::vec3(0.0, 1.0, 0.0) );
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	sphereObject.render();
}