#include <gl/glew.h>
#include <sb6/sb6.h>
#include <sb6/ktx.cpp>
#include <sb6/object.cpp>
#include <sb6/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class NormalMapping : public byhj::Application
{
public:
	NormalMapping(): program(0){}
	void v_Init();
	void v_Render();

	void init_shader();
	void init_buffer();
	void init_texture();

private:
	GLuint program, mvp_loc, mv_loc;
	Shader NormalMappingShader;
	GLuint lightPos_loc;
	sb6::Object object;
	GLuint tex_color, tex_normal;
};

CALL_MAIN(NormalMapping);

void NormalMapping::init_shader()
{
	NormalMappingShader.attach(GL_VERTEX_SHADER, "normal.vert");
	NormalMappingShader.attach(GL_FRAGMENT_SHADER, "normal.frag");
	NormalMappingShader.link();
	program = NormalMappingShader.GetProgram();
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	lightPos_loc = glGetUniformLocation(program, "lightPos");
}

void NormalMapping::v_Init()
{
	init_shader();
	init_texture();
	object.load("../../../media/objects/ladybug.sbm");
	glEnable(GL_DEPTH_TEST);
}

void NormalMapping::init_texture()
{
	glActiveTexture(GL_TEXTURE0);
	tex_color = sb6::ktx::load("../../../media/textures/ladybug_co.ktx");

	glActiveTexture(GL_TEXTURE1);
	tex_normal = sb6::ktx::load("../../../media/textures/ladybug_nm.ktx");
}
void NormalMapping::v_Render()
{
	static const GLfloat zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	static const GLfloat ones[] = { 1.0f };
	glClearBufferfv(GL_COLOR, 0, gray);
	glClearBufferfv(GL_DEPTH, 0, ones);


	float t = glfwGetTime();
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, -5.5f))
		                * glm::rotate(glm::mat4(1.0f), 14.5f, glm::vec3(1.0, 0.0, 0.0) )
						* glm::rotate(glm::mat4(1.0f), -20.0f, glm::vec3(0.0, 1.0, 0.0) );
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
	glUniform3fv(lightPos_loc, 1, &glm::vec3(40.0f * sinf(t), 30.0f + 20.0f * cosf(t), 40.0f)[0]);
	glUseProgram(program);
	object.render();
}


