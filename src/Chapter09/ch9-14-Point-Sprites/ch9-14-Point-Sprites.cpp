#include <GL/glew.h>
#include <GL/glfw3.h>
#include <cmath>
#include <iostream>
#include <vector> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ogl/oglApp.h"
#include <ogl/ktx.cpp>
#include "ogl/shader.h"


enum
{
	NUM_STARS  = 2000
};

struct  star_t//位置，颜色
{
	float  position[3];
	float  color[3];
};

class Star :public byhj::Application
{
public:
	Star():StarShader("Star shader"), program(0)
	{
	}
	~Star(){}
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();

	void v_Init()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		init_shader();
		init_buffer();
		init_vertexArray();
		init_texture();
	}

	void v_Render()
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static const GLfloat one[] = { 1.0f };

		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, one);
		glUseProgram(program);

		glm::mat4 proj_matrix = glm::perspective(50.0f, GetAspect(), 0.1f, 1000.0f);
		glUniformMatrix4fv(proj_loc, 1, 0, glm::value_ptr(proj_matrix));

		float t = glfwGetTime();
		t *= 0.1f;
		t -= floor(t);
		glBindVertexArray(star_vao);
		glUniform1f(time_loc, t);
		glBindTexture(GL_TEXTURE_2D, star_texture);

		//Enable program point size to set point size in shader
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_POINT_SPRITE);

		glDrawArrays(GL_POINTS, 0, NUM_STARS);
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}

private:
        GLuint   time_loc, proj_loc, texture;
		Shader   StarShader;
		GLuint   program;
		GLuint   star_texture;
		GLuint   star_vao;
		GLuint   star_buffer;
};

// Random number generator
static unsigned int seed = 0x13371337;

static inline float random_float() //随机数
{
	float res;
	unsigned int tmp;

	seed *= 16807;
	tmp = seed ^ (seed >> 4) ^ (seed << 15);
	*((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}


void Star::init_texture()
{
	star_texture = sb6::ktx::load("../../../media/textures/star.ktx");
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Star::init_shader()
{   
	StarShader.init();
	StarShader.attach(GL_VERTEX_SHADER, "star.vert");
	StarShader.attach(GL_FRAGMENT_SHADER, "star.frag");
	StarShader.link();
	StarShader.use();
	program = StarShader.GetProgram();
	time_loc = glGetUniformLocation(program, "time");
	proj_loc = glGetUniformLocation(program, "proj_matrix");
	glUniform1i(glGetUniformLocation(program, "tex_star"), 0);
}


void Star::init_buffer()
{

	glGenBuffers(1, &star_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, star_buffer);
	glBufferData(GL_ARRAY_BUFFER, NUM_STARS * sizeof(star_t), NULL, GL_STATIC_DRAW);
	star_t * star = (star_t *)glMapBufferRange(GL_ARRAY_BUFFER, 0, NUM_STARS * sizeof(star_t),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (int i = 0; i < 1000; i++)
	{
		star[i].position[0] = (random_float() * 2.0f - 1.0f) * 100.0f;
		star[i].position[1] = (random_float() * 2.0f - 1.0f) * 100.0f;
		star[i].position[2] = random_float();
		star[i].color[0] = 0.8f + random_float() * 0.2f;
		star[i].color[1] = 0.8f + random_float() * 0.2f;
		star[i].color[2] = 0.8f + random_float() * 0.2f;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Star::init_vertexArray()
{
	glGenVertexArrays(1, &star_vao);
	glBindVertexArray(star_vao);
	glBindBuffer(GL_ARRAY_BUFFER, star_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), (void *)sizeof(float[3]));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

CALL_MAIN(Star);
