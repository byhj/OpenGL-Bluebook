#include <gl/glew.h>
#include "ogl/oglApp.h"
#include "ogl/shader.h"
#include <ogl/vmath.h>

class QuadApp: public byhj::Application
{
public:
	QuadApp()
	{
		mode = 0;
		paused = false;
		vid_offset = 0;
	}
	~QuadApp(){}
	void init_shader();
	void v_Init()
	{
		init_shader();
	}
	void v_Render()
	{
		static const GLfloat black[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

		static double last_time = 0.0;
		static double total_time = 0.0;
		double currentTime = glfwGetTime();
		if (!paused)
			total_time += (currentTime - last_time);
		last_time = currentTime;

		float t = (float)total_time;

		glClearBufferfv(GL_COLOR, 0, black);

		vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -2.0f)
			                  * vmath::rotate((float)t * 5.0f, 0.0f, 0.0f, 1.0f)
			                  * vmath::rotate((float)t * 30.0f, 1.0f, 0.0f, 0.0f);
		vmath::mat4 proj_matrix = vmath::perspective(50.0f, GetAspect(), 0.1f, 1000.0f);
		vmath::mat4 mvp = proj_matrix * mv_matrix;

		switch (mode)
		{
		case 0:
			glUseProgram(fan_prog);
			glUniformMatrix4fv(fan_mvp_loc, 1, GL_FALSE, mvp);
			glUniform1i(fan_vid_offset_loc, vid_offset);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			break;
		case 1:
			glUseProgram(line_prog);
			glUniformMatrix4fv(line_mvp_loc, 1, GL_FALSE, mvp);
			glUniform1i(line_vid_offset_loc, vid_offset);
			glDrawArrays(GL_LINES_ADJACENCY, 0, 4);
			break;
		}
	}

	void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (key == GLFW_KEY_M && action == GLFW_PRESS)
			mode = !mode;

       if (key == GLFW_KEY_W && action == GLFW_PRESS)
			vid_offset++;

		if (key == GLFW_KEY_S && action == GLFW_PRESS)
			vid_offset--;

		if (key == GLFW_KEY_P && action == GLFW_PRESS)
			paused = !paused;
	}
private:
	GLuint fan_prog;
	GLuint line_prog;

	GLuint fan_mvp_loc;
	GLuint fan_vid_offset_loc;
	GLuint line_mvp_loc;
	GLuint line_vid_offset_loc;

	Shader FanShader;
	Shader LineShader;

	int         mode;
	int         vid_offset;
	bool        paused;
};
CALL_MAIN(QuadApp);

void QuadApp::init_shader()
{
    FanShader.init();
	FanShader.attach( GL_VERTEX_SHADER , "fan.vert");
	FanShader.attach( GL_FRAGMENT_SHADER, "fan.frag");
    FanShader.link();
	fan_prog = FanShader.GetProgram();

	fan_mvp_loc = glGetUniformLocation(fan_prog, "mvp");
	fan_vid_offset_loc= glGetUniformLocation(fan_prog, "vid_offset");

	LineShader.init();
	LineShader.attach(GL_VERTEX_SHADER  , "line.vert");
	LineShader.attach(GL_GEOMETRY_SHADER, "line.geom");
	LineShader.attach(GL_FRAGMENT_SHADER, "line.frag");
	line_prog = LineShader.GetProgram();

	line_mvp_loc = glGetUniformLocation(line_prog, "mvp");
	line_vid_offset_loc= glGetUniformLocation(line_prog, "vid_offset");
}