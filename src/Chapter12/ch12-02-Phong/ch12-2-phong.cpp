#include <gl/glew.h>
#include "ogl/oglApp.h"
#include <ogl/object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/shader.h"


class Phong: public byhj::Application
{
public:
	void v_Init();
	void v_Render();

	void init_shader();
	void init_buffer();

private:
	GLuint program, mvp_loc, mv_loc;
	Shader PhongShader;
	sb6::Object sphereObject;
	GLuint 	specular_power, specular_albedo;
};
CALL_MAIN(Phong);

void Phong::init_shader()
{
	PhongShader.attach(GL_VERTEX_SHADER, "Phong.vert");
	PhongShader.attach(GL_FRAGMENT_SHADER, "Phong.frag");
	PhongShader.link();
	program = PhongShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	specular_power = glGetUniformLocation(program, "specular_power");
	specular_albedo = glGetUniformLocation(program, "specular_albedo");
}

void Phong::v_Init()
{
	init_shader();
	sphereObject.load("../../../media/objects/sphere.sbm");
	glEnable(GL_DEPTH_TEST);
}

void Phong::v_Render()
{
	static const float one[] = {1.0f};
	static const float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, one);

	glUseProgram(program);
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0, 0.0, 18.0), 
		                                glm::vec3(0.0f, 0.0f, 0.0f),
										glm::vec3(0.0f, 1.0f, 0.0f));
#ifdef MANY_SPHERE
	for (int j = 0; j < 7; j++)
	{
		for (int i = 0; i < 7; i++)
		{

			float t = glfwGetTime() / 100.0;
			glm::mat4 mv_matrix = view_matrix 
				* glm::translate(glm::mat4(1.0), glm::vec3((float)i * 3.0f - 9.25f, 6.75f - (float)j * 2.4f, 0.0f)) 
				* glm::rotate(glm::mat4(1.0f), sin(t) * 60.0f, glm::vec3(0.0, 1.0, 0.0) );
			glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
			glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
			glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
			glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);

			glUniform1f(specular_power, powf(2.0f, (float)j + 2.0f));
			glUniform3fv(specular_albedo, 1, glm::value_ptr(glm::vec3((float)i / 9.0f + 1.0f / 9.0f)));
			sphereObject.render();
		}
	}
#endif
	float t = glfwGetTime() / 1000.0;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.5f))
		* glm::rotate(glm::mat4(1.0f), sin(t) * 60.0f, glm::vec3(0.0, 1.0, 0.0) );
	glm::mat4 proj_matrix = glm::perspective(45.0f, GetAspect(), 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	sphereObject.render();
}