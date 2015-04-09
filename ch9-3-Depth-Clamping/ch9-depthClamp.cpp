#include <gl/glew.h>
#include <gl/glfw3.h>
#include <shader.h>
#include <sb6.h>
#include <object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Clamping: public sb6::Application
{
public:
	Clamping(){};
	virtual ~Clamping(){};
public:
	virtual void init();
	virtual void render();
	void init_buffer();
	void init_shader();
private:
	GLuint program;
	GLuint mv_loc, mvp_loc, vp_loc;
	Shader ClampingShader;
	Object object;
};

DECLARE_MAIN(Clamping);

void Clamping::init_shader()
{
	ClampingShader.attach(GL_VERTEX_SHADER, "depthClamp.vert");
	ClampingShader.attach(GL_FRAGMENT_SHADER, "depthClamp.frag");
	ClampingShader.link();
	program = ClampingShader.program;
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

void Clamping::init()
{
	init_shader();
	object.load("../media/objects/bunny_1k.sbm");
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Clamping::render()
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;

	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);
    glEnable(GL_DEPTH_CLAMP);  //…Ó∂»≤Â÷µ

	float f = glfwGetTime() / 100.0;
	glUseProgram(program);
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -0.5f)) 
		* glm::rotate(glm::mat4(1.0), f* 45.0f, glm::vec3(0.0f, 1.0f, 0.0f)) 
		* glm::rotate(glm::mat4(1.0), f* 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_matrix));
	object.render();
}
