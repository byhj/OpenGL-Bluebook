#include "Geometry.h"
#include <glfw/glfw3.h>
#include "ogl/oglDebug.h"

#include "ogl/vmath.h"

namespace byhj
{

Geometry::Geometry()
{

}

Geometry::~Geometry()
{

}

void Geometry::Init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
}

void Geometry::Render(GLfloat aspect)
{

	static double last_time = 0.0;
	static double total_time = 0.0;
	double currentTime = glfwGetTime();
	if (!paused)
		total_time += (currentTime - last_time);
	last_time = currentTime;

	float t = (float)total_time;


	vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -2.0f)
		* vmath::rotate((float)t * 5.0f, 0.0f, 0.0f, 1.0f)
		* vmath::rotate((float)t * 30.0f, 1.0f, 0.0f, 0.0f);
	vmath::mat4 proj_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);
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

	glUseProgram(0);
}

void Geometry::Shutdown()
{
}



void Geometry::init_buffer()
{
}

void Geometry::init_vertexArray()
{


}


void Geometry::init_shader()
{
	FanShader.init();
	FanShader.attach(GL_VERTEX_SHADER, "fan.vert");
	FanShader.attach(GL_FRAGMENT_SHADER, "fan.frag");
	FanShader.link();
	fan_prog = FanShader.GetProgram();

	fan_mvp_loc = glGetUniformLocation(fan_prog, "mvp");
	fan_vid_offset_loc= glGetUniformLocation(fan_prog, "vid_offset");

	LineShader.init();
	LineShader.attach(GL_VERTEX_SHADER, "line.vert");
	LineShader.attach(GL_GEOMETRY_SHADER, "line.geom");
	LineShader.attach(GL_FRAGMENT_SHADER, "line.frag");
	line_prog = LineShader.GetProgram();

	line_mvp_loc = glGetUniformLocation(line_prog, "mvp");
	line_vid_offset_loc= glGetUniformLocation(line_prog, "vid_offset");
}

}