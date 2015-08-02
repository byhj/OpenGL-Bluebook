#include "Plane.h"
#include <glfw/glfw3.h>
#include "ogl/oglDebug.h"
#include "ogl/vmath.h"


namespace byhj
{

Plane::Plane()
{

}

Plane::~Plane()
{

}

void Plane::Init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
}



void Plane::Render(GLfloat aspect)
{
	static float currentTime;
	currentTime = glfwGetTime();
	glUseProgram(program);

	//set the MVP matrix
	float time = (float)currentTime * (float)3.14159 * 0.1f;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1.5f))
		* glm::rotate(glm::mat4(1.0), currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
	glUniformMatrix4fv(mvp_matrix_loc, 1, 0, &mvp_matrix[0][0]);

	//We use PerspectiveApp correction default
	glUniform1i(perspective_loc, use_perspective);

	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(0);


}

void Plane::Shutdown()
{

}


void Plane::init_shader()
{
	perspectiveShader.init();
	perspectiveShader.attach(GL_VERTEX_SHADER, "perspective.vert");
	perspectiveShader.attach(GL_FRAGMENT_SHADER, "perspective.frag");
	perspectiveShader.link();
	perspectiveShader.info();
	perspectiveShader.use();
	program = perspectiveShader.GetProgram();

	perspective_loc = glGetUniformLocation(program, "use_perspective");
	mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");
	tex_loc = glGetUniformLocation(program, "tex_checker");
	glUniform1i(tex_loc, 0);

}


void Plane::init_vertexArray()
{

}

void Plane::init_buffer()
{

}

#define BWBW 0x00, 0xFF, 0x00, 0xFF
#define WBWB 0xFF, 0x00, 0xFF, 0x00

const static unsigned char checker_data[] ={
	WBWB, WBWB,
	BWBW, BWBW,
	WBWB, WBWB,
	BWBW, BWBW,
	WBWB, WBWB,
	BWBW, BWBW,
	WBWB, WBWB,
	BWBW, BWBW
};
void Plane::init_texture()
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, 8, 8); //LEVEL must set >=1

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 8, 8, GL_RED, GL_UNSIGNED_BYTE, checker_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

}

}