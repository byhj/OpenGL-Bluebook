#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <object.cpp>
#include <ktx.cpp>

class Clip :public sb6::Application
{
public:
	Clip():ClipShader("Clip shader"), program(0){}
	~Clip(){}
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_Clip();

	virtual void init()
	{
		init_shader();
		object.load("../media/objects/dragon.sbm");
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CLIP_DISTANCE0);
		glEnable(GL_CLIP_DISTANCE1);
	}

	virtual void render()
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static const GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);

		glUseProgram(program);
		float currTime = static_cast<float>(glfwGetTime()) / 10.0f;
		glm::mat4 mat = glm::mat4(1.0f);
		glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
		glm::mat4 mv_matrix = glm::translate(mat, glm::vec3(0.0f, 0.0f, -15.0f)) 
			                * glm::rotate(mat,  currTime * 0.34f, glm::vec3(0.0f, 1.0f, 0.0f) ) 
			                * glm::translate(mat, glm::vec3(0.0f, -4.0f, 0.0f));

		glm::mat4 plane_matrix = glm::rotate(mat, currTime* 6.0f, glm::vec3(1.0f, 0.0f, 0.0f)) 
			                   * glm::rotate(mat, currTime * 7.3f, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec4 plane = plane_matrix[0];
		plane[3] = 0.0f;
		plane = glm::normalize(plane);

		glm::vec4 clip_sphere = glm::vec4(sinf(currTime * 0.7f) * 3.0f, 
			                              cosf(currTime* 1.9f) * 3.0f, 
			                              sinf(currTime * 0.1f) * 3.0f, 
			                              cosf(currTime * 1.7f) + 2.5f);

		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj_matrix[0][0]);
		glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
		glUniform4fv(clip_plane_loc, 1, &plane[0]);
		glUniform4fv(clip_sphere_loc, 1, &clip_sphere[0]);

		glUseProgram(program);
		object.render();
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}
private:
	GLuint program;
	Shader ClipShader;
	Object object;
	GLint   proj_loc;
	GLint   mv_loc;
	GLint   clip_plane_loc;
	GLint   clip_sphere_loc;
};



void Clip::init_Clip()
{
	GLuint Clip;

}


void Clip::init_shader()
{
	ClipShader.attach(GL_VERTEX_SHADER, "Clip.vert");
	ClipShader.attach(GL_FRAGMENT_SHADER, "Clip.frag");

	ClipShader.link();
	program = ClipShader.program;
	glUseProgram(program);
	proj_loc = glGetUniformLocation(program, "proj_matrix");
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	clip_plane_loc = glGetUniformLocation(program, "clip_plane");
	clip_sphere_loc = glGetUniformLocation(program, "clip_sphere");
}

void Clip::init_buffer()
{

}

void Clip::init_vertexArray()
{
}

DECLARE_MAIN(Clip);