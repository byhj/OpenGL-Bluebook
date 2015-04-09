#include <gl/glew.h>
#include <gl/glfw3.h>
#include <shader.h>
#include <sb6.h>
#include <object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Culling: public sb6::Application
{
public:
	Culling(){};
	virtual ~Culling(){};
public:
	virtual void init();
	virtual void render();
	void init_buffer();
	void init_shader();
private:
	GLuint program;
	GLuint mv_loc, mvp_loc, explode_factor_loc;
	Shader CullingShader;
	Object object;
};

DECLARE_MAIN(Culling);

void Culling::init_shader()
{
	CullingShader.attach(GL_VERTEX_SHADER, "culling.vert");
	CullingShader.attach(GL_GEOMETRY_SHADER, "culling.geom");
	CullingShader.attach(GL_FRAGMENT_SHADER, "culling.frag");
	CullingShader.link();
	program = CullingShader.program;
    mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	explode_factor_loc = glGetUniformLocation(program, "explode_factor");
}

void Culling::init()
{
	init_shader();
	object.load("../media/objects/bunny_1k.sbm");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Culling::render()
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;

	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glUseProgram(program);
	float f = glfwGetTime() / 1000.0f;
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1.5f)) 
	                    * glm::rotate(glm::mat4(1.0), (float)f * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	                    //* glm::rotate(glm::mat4(1.0), (float)f * 81.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp_matrix));
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_matrix));

	object.render();
}