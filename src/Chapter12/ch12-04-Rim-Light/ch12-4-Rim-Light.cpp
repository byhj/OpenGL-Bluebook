#include <gl/glew.h>
#include <sb6/sb6.h>
#include <sb6/object.cpp>
#include <sb6/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class RimLight : public byhj::Application
{
public:
	RimLight()
		: program(0),
		paused(false),
		rim_color(0.3f, 0.3f, 0.3f),
		rim_power(2.5f),
		rim_enable(true){}

	void v_Init();
	void init_shader();
	void v_Render();
	void init_buffer();
	void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode);
private:
	GLuint program, mvp_loc, mv_loc;
	Shader RimLightShader;
	GLint           rim_color_loc;
	GLint           rim_power_loc;

	glm::mat4  mat_rotation;
	sb6::Object      object;
	bool        paused;
	glm::vec3   rim_color;
	float       rim_power;
	bool        rim_enable;
};

CALL_MAIN(RimLight);

void RimLight::init_shader()
{
	RimLightShader.attach(GL_VERTEX_SHADER, "rim.vert");
	RimLightShader.attach(GL_FRAGMENT_SHADER, "rim.frag");
	RimLightShader.link();
	program = RimLightShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	rim_color_loc = glGetUniformLocation(program, "rim_color");
	rim_power_loc = glGetUniformLocation(program, "rim_power");
}

void RimLight::v_Init()
{
	init_shader();
	object.load("../../../media/objects/dragon.sbm");
	glEnable(GL_DEPTH_TEST);
}

void RimLight::v_Render()
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
	glm::mat4 proj_matrix = glm::perspective(45.0f, GetAspect(), 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);

	glUseProgram(program);
	object.render();
}

void RimLight::v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
    if (action == GLFW_PRESS)
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

