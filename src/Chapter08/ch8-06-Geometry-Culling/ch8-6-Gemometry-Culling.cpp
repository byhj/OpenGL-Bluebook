#include <ogl/shader.h>
#include "ogl/oglApp.h"
#include <ogl/object.cpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Culling: public byhj::Application
{
public:
	Culling(){};
	virtual ~Culling(){};

public:
	void v_Init();
	void v_Render();
	void init_buffer();
	void init_shader();

private:
	GLuint program;
	GLuint mv_loc, mvp_loc, vp_loc;
	Shader CullingShader;
	sb6::Object object;
};

CALL_MAIN(Culling);

void Culling::init_shader()
{
	CullingShader.attach(GL_VERTEX_SHADER, "culling.vert");
	CullingShader.attach(GL_GEOMETRY_SHADER, "culling.geom");
	CullingShader.attach(GL_FRAGMENT_SHADER, "culling.frag");
	CullingShader.link();
	program = CullingShader.GetProgram();

    mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	vp_loc = glGetUniformLocation(program, "viewpoint");
}

void Culling::v_Init()
{
	init_shader();
	object.load("../../../media/objects/bunny_1k.sbm");

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Culling::v_Render()
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;

	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	static float f = glfwGetTime();
	f += 0.01;

	glUseProgram(program);
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, GetAspect(), 0.1f, 1000.0f);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_matrix));

	//different viewpoint
	GLfloat vViewpoint[] = {sinf(f * 2.1f) * 70.0f, cosf(f * 1.4f) * 70.0f, sinf(f * 0.7f) * 70.0f };
	glUniform3fv(vp_loc, 1, vViewpoint);
	object.render();

}