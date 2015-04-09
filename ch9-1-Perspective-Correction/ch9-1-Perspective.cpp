#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>
#include <sb6.h>

#define BWBW 0x00, 0xFF, 0x00, 0xFF
#define WBWB 0xFF, 0x00, 0xFF, 0x00

const static unsigned char checker_data[] = {
	WBWB, WBWB,
	BWBW, BWBW,
	WBWB, WBWB,
	BWBW, BWBW,
	WBWB, WBWB,
	BWBW, BWBW,
	WBWB, WBWB,
	BWBW, BWBW
};

class Perspective: public sb6::Application
{
public:
	void init();
	void render();
	void init_shader();
	void init_buffer();
	void init_texture();
	void init_vertexArray();
private:
	 GLuint perspective_loc, mvp_matrix_loc;
     GLuint program, tex;
     bool use_perspective;
     Shader perspectiveShader;
};

DECLARE_MAIN(Perspective);

void Perspective::init_shader()
{
	perspectiveShader.attach(GL_VERTEX_SHADER, "perspective.vert");
	perspectiveShader.attach(GL_FRAGMENT_SHADER, "perspective.frag");
	perspectiveShader.link();
	perspectiveShader.use();
	program = perspectiveShader.program;
	perspective_loc = glGetUniformLocation(program, "use_perspective"); 
	mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");
	glUniform1i(glGetUniformLocation(program, "tex_checker"), 0);
}

void Perspective::init_texture()
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_R8, 8, 8);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 8, 8, GL_RED, GL_UNSIGNED_BYTE, checker_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Perspective::init()
{
	init_shader();
	init_texture();
}

void Perspective::render()
{
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)[0]);

	static float currentTime ;
	currentTime = glfwGetTime() / 1000.0f;

	float time = (float)currentTime * (float)3.14159 * 0.1f;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1.5f))
		* glm::rotate(glm::mat4(1.0), currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, 720.0f / 640.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix ; 
	glUniformMatrix4fv(mvp_matrix_loc, 1, 0, &mvp_matrix[0][0]);
	glUniform1i(perspective_loc, use_perspective);
	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}




