#include "Clip.h"
#include "ogl/vmath.h"

#include <glfw/glfw3.h>

namespace byhj
{

Clip::Clip()
{

}

Clip::~Clip()
{

}

void Clip::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CLIP_DISTANCE1);

	init_buffer();
	init_vertexArray();
	init_shader();
}

void Clip::Render(GLfloat aspect)
{

	float f = (float)glfwGetTime();

	glUseProgram(program);
	float currTime = static_cast<float>(glfwGetTime()) / 10.0f;

	vmath::mat4 proj_matrix = vmath::perspective(50.0f,aspect, 0.1f, 1000.0f);

	vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -15.0f)
		* vmath::rotate(f * 0.34f, 0.0f, 1.0f, 0.0f)
		* vmath::translate(0.0f, -4.0f, 0.0f);

	vmath::mat4 plane_matrix = vmath::rotate(f * 6.0f, 1.0f, 0.0f, 0.0f)
		* vmath::rotate(f * 7.3f, 0.0f, 1.0f, 0.0f);

	vmath::vec4 plane = plane_matrix[0];
	plane[3] = 0.0f;
	plane = vmath::normalize(plane);
	vmath::vec4 clip_sphere = vmath::vec4(sinf(f * 0.7f) * 3.0f, cosf(f * 1.9f) * 3.0f,
		sinf(f * 0.1f) * 3.0f, cosf(f * 1.7f) + 2.5f);

	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, mv_matrix);
	glUniform4fv(clip_plane_loc, 1, plane);
	glUniform4fv(clip_sphere_loc, 1, clip_sphere);

	object.render();

	glUseProgram(0);
}


void Clip::Shutdown()
{
	glDeleteProgram(program);
}

void Clip::init_buffer()
{
	object.load("../../../media/objects/dragon.sbm");
}

void Clip::init_vertexArray()
{

}

void Clip::init_shader()
{
	ClipShader.init();
	ClipShader.attach(GL_VERTEX_SHADER, "Clip.vert");
	ClipShader.attach(GL_FRAGMENT_SHADER, "Clip.frag");
	ClipShader.link();
	program = ClipShader.GetProgram();

	glUseProgram(program);
	proj_loc = glGetUniformLocation(program, "proj_matrix");
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	clip_plane_loc = glGetUniformLocation(program, "clip_plane");
	clip_sphere_loc = glGetUniformLocation(program, "clip_sphere");
}

}