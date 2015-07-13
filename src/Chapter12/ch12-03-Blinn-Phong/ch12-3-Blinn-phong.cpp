#include <gl/glew.h>
#include "ogl/oglApp.h"
#include <ogl/object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ogl/shader.h>


class Blinn: public byhj::Application
{
public:
	void v_Init();
	void v_Render();

	void init_shader();
	void init_buffer();

private:
	GLuint program, mvp_loc, mv_loc;
	Shader BlinnShader;
	sb6::Object sphereObject;
};
CALL_MAIN(Blinn);

void Blinn::init_shader()
{
	BlinnShader.attach(GL_VERTEX_SHADER, "Blinn.vert");
	BlinnShader.attach(GL_FRAGMENT_SHADER, "Blinn.frag");
	BlinnShader.link();
	program = BlinnShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

void Blinn::v_Init()
{
	init_shader();
	sphereObject.load("../../../media/objects/sphere.sbm");
	glEnable(GL_DEPTH_TEST);
}

void Blinn::v_Render()
{
	static const float one[] = {1.0f};
	static const float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, one);

	glUseProgram(program);

	float t = glfwGetTime() / 1000.0;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.5f))
		* glm::rotate(glm::mat4(1.0f), sin(t) * 60.0f, glm::vec3(0.0, 1.0, 0.0) );
	glm::mat4 proj_matrix = glm::perspective(45.0f, GetAspect(), 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	sphereObject.render();
}