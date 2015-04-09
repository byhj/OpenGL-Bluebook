#include <GL/glew.h>
#include <GL/glfw3.h>
#include <shader.h>
#include <sb6.h>
#include <ktx.cpp>
#include <iostream>
#include <vector> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Terrian :public sb6::Application
{
public:
	void init() { init_shader(); }
	void render();
	void init_shader();
private:

GLuint          program;
GLuint          vao;
GLuint          tex_displacement;
GLuint          tex_color;
float           dmap_depth;
bool            enable_displacement;
bool            wireframe;
bool            enable_fog;
bool            paused;

Shader dispmapShader;
struct
{
	GLint       mvp_matrix;
	GLint       mv_matrix;
	GLint       proj_matrix;
	GLint       dmap_depth;
	GLint       enable_fog;
} uniforms;
};
DECLARE_MAIN(Terrian);


void Terrian::init_shader()  
{ 

	dispmapShader.attach(GL_VERTEX_SHADER, "dispmap.vert");
	dispmapShader.attach(GL_TESS_CONTROL_SHADER, "dispmap.tcs");
	dispmapShader.attach(GL_TESS_EVALUATION_SHADER, "dispmap.tes");
	dispmapShader.attach(GL_FRAGMENT_SHADER, "dispmap.frag");
	dispmapShader.link();
	program = dispmapShader.program;

	uniforms.mv_matrix = glGetUniformLocation(program, "mv_matrix");
	uniforms.mvp_matrix = glGetUniformLocation(program, "mvp_matrix");
	uniforms.proj_matrix = glGetUniformLocation(program, "proj_matrix");
	uniforms.dmap_depth = glGetUniformLocation(program, "dmap_depth");
	uniforms.enable_fog = glGetUniformLocation(program, "enable_fog");
	dmap_depth = 6.0f;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glEnable(GL_CULL_FACE);
	tex_displacement = ktx::file::load("../media/textures/terragen1.ktx");
	glActiveTexture(GL_TEXTURE1);
	tex_color = ktx::file::load("../media/textures/terragen_color.ktx");

	uniforms.mv_matrix = glGetUniformLocation(program, "mv_matrix");
	uniforms.proj_matrix = glGetUniformLocation(program, "proj_matrix");
}

void Terrian::render(void)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	static const GLfloat black[] = { 0.85f, 0.95f, 1.0f, 1.0f };
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	float currentTime =  glfwGetTime() / 100.0f;

	float t = (float)currentTime * 0.03f;
	float r = sinf(t * 5.37f) * 15.0f + 16.0f;
	float h = cosf(t * 4.79f) * 2.0f + 3.2f;


	glm::mat4 mv_matrix  = glm::lookAt( glm::vec3(sinf(t) * r, h, cosf(t) * r), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));   
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);

	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, glm::value_ptr(proj_matrix));
	glUniformMatrix4fv(uniforms.mvp_matrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
	glUniform1f(uniforms.dmap_depth, enable_displacement ? dmap_depth : 0.0f);
	glUniform1i(uniforms.enable_fog, enable_fog ? 1 : 0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

