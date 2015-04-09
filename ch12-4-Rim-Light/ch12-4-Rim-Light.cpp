#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>

class RimLight : public sb6::Application
{
public:
	RimLight()
		: program(0),
		paused(false),
		rim_color(0.3f, 0.3f, 0.3f),
		rim_power(2.5f),
		rim_enable(true){}

	void init();
	void init_shader();
	void render();
	void init_buffer();
	void keyboard(GLFWwindow * window, int key, int scancode, int action, int mode);
private:
	GLuint program, mvp_loc, mv_loc;
	Shader RimLightShader;
	GLint           rim_color_loc;
	GLint           rim_power_loc;

	glm::mat4  mat_rotation;
	Object      object;
	bool        paused;
	glm::vec3   rim_color;
	float       rim_power;
	bool        rim_enable;
};

DECLARE_MAIN(RimLight);

void RimLight::init_shader()
{
	RimLightShader.attach(GL_VERTEX_SHADER, "rim.vert");
	RimLightShader.attach(GL_FRAGMENT_SHADER, "rim.frag");
	RimLightShader.link();
	program = RimLightShader.program;
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	rim_color_loc = glGetUniformLocation(program, "rim_color");
	rim_power_loc = glGetUniformLocation(program, "rim_power");
}

void RimLight::init()
{
	init_shader();
	object.load("../media/objects/dragon.sbm");
	glEnable(GL_DEPTH_TEST);
}

void RimLight::render()
{
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat one = 1.0f;
	static double last_time = 0.0;
	static double total_time = 0.0;

	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glUniform3fv(rim_color_loc, 1, &(rim_enable ? rim_color : glm::vec3(0.0f))[0] );
	glUniform1f(rim_power_loc, rim_power);

	float t = glfwGetTime() / 1000.0;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, -20.0f))
		* glm::rotate(glm::mat4(1.0f), sin(t) * 60.0f, glm::vec3(0.0, 1.0, 0.0) );
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);

	glUseProgram(program);
	object.render();
}

void RimLight::keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
    if (action)
    {
            switch (key)
            {
                case 'Q':
                    rim_color[0] += 0.1f; break;
                case 'W':
                    rim_color[1] += 0.1f; break;
                case 'E':
                    rim_color[2] += 0.1f; break;
                case 'R':
                    rim_power *= 1.5f;    break;
                case 'A':
                    rim_color[0] -= 0.1f; break;
                case 'S':
                    rim_color[1] -= 0.1f; break;
                case 'D':
                    rim_color[2] -= 0.1f; break;
                case 'F':
                    rim_power /= 1.5f;    break;
                case 'Z':
                    rim_enable = !rim_enable; break;
            }
     }
}

