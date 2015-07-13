#include <gl/glew.h>
#include "ogl/oglApp.h"
#include <ogl/shader.h>
#include <ogl/object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


static const GLubyte toon_tex_data[] =
{
	0x44, 0x00, 0x00, 0x00,
	0x88, 0x00, 0x00, 0x00,
	0xCC, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0x00
};

class Toon: public byhj::Application
{
public:
	void v_Init();
	void init_shader();
	void v_Render();
	void init_buffer();
	void init_texture();
private:
	GLuint program, mvp_loc, mv_loc, tex_toon;
	Shader ToonShader;
	sb6::Object sphereObject;
};
CALL_MAIN(Toon);

void Toon::init_texture()
{

	glGenTextures(1, &tex_toon);
	glBindTexture(GL_TEXTURE_1D, tex_toon);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB8, sizeof(toon_tex_data) / 4);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, sizeof(toon_tex_data) / 4,
		            GL_RGBA, GL_UNSIGNED_BYTE, toon_tex_data);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
}
void Toon::init_shader()
{
	ToonShader.attach(GL_VERTEX_SHADER, "Toon.vert");
	ToonShader.attach(GL_FRAGMENT_SHADER, "Toon.frag");
	ToonShader.link();
	program = ToonShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

void Toon::v_Init()
{
	init_shader();
	init_texture();
	sphereObject.load("../../../media/objects/torus_nrms_tc.sbm");
	glEnable(GL_DEPTH_TEST);
}

void Toon::v_Render()
{
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	static const GLfloat ones[] = { 1.0f };

	glClearBufferfv(GL_COLOR, 0, gray);
	glClearBufferfv(GL_DEPTH, 0, ones);

	glBindTexture(GL_TEXTURE_1D, tex_toon);

	float t = glfwGetTime() / 100.0;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))
		* glm::rotate(glm::mat4(1.0f), (float)t * 13.75f, glm::vec3(0.0, 1.0, 0.0) )
		* glm::rotate(glm::mat4(1.0f), (float)t * 87.75f, glm::vec3(0.0, 0.0, 1.0) )
		* glm::rotate(glm::mat4(1.0f), (float)t * 15.3f, glm::vec3(1.0, 0.0, 0.0) );
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);

	glUseProgram(program);
	sphereObject.render();
}