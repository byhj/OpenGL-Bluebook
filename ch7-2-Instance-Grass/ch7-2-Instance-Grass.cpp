#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ktx.cpp>

class Grass :public sb6::Application
{
public:
	Grass():GrassShader("Grass shader"), program(0){}
	~Grass(){}
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();

	virtual void init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();

		init_texture();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	virtual void render()
	{

		float t = (float)glfwGetTime() * 0.02f;
		float r = 550.0f;

		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);

		glm::mat4 mv_matrix = glm::lookAt(glm::vec3(sinf(t) * r, 25.0f, cosf(t) * r),
			                  glm::vec3(0.0f, -50.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);

		glUseProgram(program);
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &(proj_matrix * mv_matrix)[0][0]);

		glBindVertexArray(vao);           //1024  1024
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 1024 * 1024);
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}

private:
	GLuint program;
	Shader GrassShader;
	GLuint mvp_loc, vao, vbo;
	GLuint tex_grass_color;
	GLuint tex_grass_length;
	GLuint tex_grass_orientation;
	GLuint tex_grass_bend;
};

static const GLfloat grass_blade[] =
{
	-0.3f, 0.0f,
	0.3f, 0.0f,
	-0.20f, 1.0f,
	0.1f, 1.3f,
	-0.05f, 2.3f,
	0.0f, 3.3f
};

void Grass::init_texture()
{
	glActiveTexture(GL_TEXTURE1);
	tex_grass_length = ktx::file::load("../media/textures/grass_length.ktx");
	glActiveTexture(GL_TEXTURE2);
	tex_grass_orientation = ktx::file::load("../media/textures/grass_orientation.ktx");
	glActiveTexture(GL_TEXTURE3);
	tex_grass_color = ktx::file::load("../media/textures/grass_color.ktx");
	glActiveTexture(GL_TEXTURE4);
	tex_grass_bend =  ktx::file::load("../media/textures/grass_bend.ktx");
}


void Grass::init_shader()
{
	GrassShader.attach(GL_VERTEX_SHADER, "grass.vert");
	GrassShader.attach(GL_FRAGMENT_SHADER, "grass.frag");
	GrassShader.link();
	program = GrassShader.program;
	glUseProgram(program);
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}


void Grass::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grass_blade), grass_blade, GL_STATIC_DRAW);
}

void Grass::init_vertexArray()
{	
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}

DECLARE_MAIN(Grass);