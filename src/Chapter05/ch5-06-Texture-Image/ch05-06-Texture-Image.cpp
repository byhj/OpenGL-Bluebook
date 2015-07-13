#include <GL/glew.h>
#include "ogl/oglApp.h"
#include <ogl/shader.h>
#include <ogl/ktx.cpp>
#include <ogl/object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ogl/glDebug.h>

class TriangleApp: public byhj::Application
{
public:
	TriangleApp():program(0), TriangleShader("Triangle Shader"), tex_index(0) 
	{
		memset(keys, 0, sizeof(keys));
	};

	~TriangleApp() {} ;

	void v_Init()
	{
		init_texture();
		init_shader();
		init_buffer();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

	}

	void v_Render()
	{
		static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
		static const GLfloat ones[] = { 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, ones);

		double currentTime = glfwGetTime();
		glUseProgram(program);

		glBindTexture(GL_TEXTURE_2D, texture[tex_index]);

		glm::mat4 mvp = glm::perspective(45.0f, (float)windowInfo.Width / (float)windowInfo.Height, 0.1f, 1000.0f)
			          * glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f))
			          * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) 
			          * glm::rotate(glm::mat4(1.0f), glm::radians( (float)currentTime * 19.3f ) , glm::vec3(0.0f, 1.0f, 0.0f)) 
			          * glm::rotate(glm::mat4(1.0f), glm::radians( (float)currentTime * 21.1f ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

		obj.render();

		glUseProgram(0);
	}

	void v_Shutdown()
	{
		glDeleteProgram(program);
	}

	void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (key == GLFW_KEY_T && action == GLFW_PRESS)
				tex_index = !tex_index;
	}
private:

	void init_shader();
	void init_texture();
	void init_buffer();

private:
	Shader TriangleShader;
	sb6::Object obj;
	GLuint program;
	GLuint texture[2], tex_loc, tex_index;
	GLuint mvp_loc;
	bool keys[1024];
};

CALL_MAIN(TriangleApp);

#pragma region  TextureData

//Checkerbox texture
#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
static const GLubyte tex_data[] =  
{
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
};
#undef B
#undef W

#pragma endregion

void TriangleApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "texture.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "texture.frag");
	TriangleShader.link();
	TriangleShader.interfaceInfo();

	program = TriangleShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");
	mvp_loc = glGetUniformLocation(program, "mvp");
	glUseProgram(program);
	glUniform1i(tex_loc, 0);
	glUseProgram(0);
}

void TriangleApp::init_texture()
{
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 16, 16);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	texture[1] = sb6::ktx::load("../../../media/textures/pattern1.ktx");

	glBindTexture(GL_TEXTURE_2D, 0);

}

void TriangleApp::init_buffer()
{
	obj.load("../../../media/objects/torus_nrms_tc.sbm");
}