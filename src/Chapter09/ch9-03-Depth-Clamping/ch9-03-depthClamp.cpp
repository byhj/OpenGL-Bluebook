#include <gl/glew.h>
#include "ogl/shader.h"
#include "ogl/oglApp.h"
#include "ogl/object.cpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Clamping: public byhj::Application
{
public:
	Clamping(){ depthClamp = true; }
	virtual ~Clamping(){};

public:
	void v_Init();
	void v_Render();
	void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
			depthClamp = !depthClamp;
	}
	void init_buffer();
	void init_shader();
private:

	GLuint program;
	GLuint mv_loc, mvp_loc, vp_loc;
	Shader ClampingShader;
	sb6::Object object;
	bool depthClamp;
};

CALL_MAIN(Clamping);

void Clamping::init_shader()
{
	ClampingShader.attach(GL_VERTEX_SHADER, "depthClamp.vert");
	ClampingShader.attach(GL_FRAGMENT_SHADER, "depthClamp.frag");
	ClampingShader.link();
	program = ClampingShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

void Clamping::v_Init()
{
	init_shader();
	object.load("../../../media/objects/bunny_1k.sbm");
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Clamping::v_Render()
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;

	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	if (depthClamp)
	  glEnable(GL_DEPTH_CLAMP);
	else
	 glDisable(GL_DEPTH_CLAMP);
	 float f = glfwGetTime();
	glUseProgram(program);
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -0.3f)) 
		                * glm::rotate(glm::mat4(1.0), glm::radians(f* 45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) 
		                * glm::rotate(glm::mat4(1.0), glm::radians(f* 81.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 proj_matrix = glm::perspective(45.0f, GetAspect(), 0.1f, 1000.0f);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_matrix));

	object.render();
}
