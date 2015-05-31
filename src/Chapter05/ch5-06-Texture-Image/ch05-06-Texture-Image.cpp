#include <GL/glew.h>
#include <sb6/sb6.h>
#include <sb6/shader.h>
#include <sb6/ktx.cpp>
#include <sb6/object.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TriangleApp: public sb6::Application
{
public:
	TriangleApp():program(0), TriangleShader("Triangle Shader"), tex_index(0) 
	{
		memset(keys, 0, sizeof(keys));
	};

	~TriangleApp() {} ;

	void vInit()
	{
		init_texture();
		init_shader();
		obj.load("../../../media/objects/torus_nrms_tc.sbm");
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	void vRender()
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

	void vShutdown()
	{
		glDeleteProgram(program);
	}

	void keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		//Enter Key 't' to change the texture
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS && key == GLFW_KEY_T)
				tex_index = !tex_index;
		}
	}

	void init_shader();
	void init_texture();

private:
	Shader TriangleShader;
	sb6::Object obj;
	GLuint program;
	GLuint texture[2], tex_loc, tex_index;
	GLuint mvp_loc;
	bool keys[1024];
};

DECLARE_MAIN(TriangleApp);

//Checkbox texture
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


void TriangleApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "texture.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "texture.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");
	mvp_loc = glGetUniformLocation(program, "mvp");
	glUniform1i(tex_loc, 0);
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