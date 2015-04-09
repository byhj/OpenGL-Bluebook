#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <object.cpp>
#include <ktx.cpp>

#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
static const GLubyte tex_data[] =  //ÆåÅÌºÚ°×ÎÆÀí
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

class Texture :public sb6::Application
{
public:
	Texture():TextureShader("Texture shader"), program(0), tex_index(0){
		memset(keys, 0, sizeof(keys));
	}
	~Texture(){}
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();

	virtual void init()
	{
		init_shader();
		init_texture();
		object.load("../media/objects/torus_nrms_tc.sbm");
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	virtual void render()
	{
		static const GLfloat gray[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		static const GLfloat ones[] = { 1.0f };
		glClearBufferfv(GL_COLOR, 0, gray);
		glClearBufferfv(GL_DEPTH, 0, ones);

		float currentTime = glfwGetTime();
		glUseProgram(program);
		glBindTexture(GL_TEXTURE_2D, tex[tex_index]);
		glm::mat4 mvp = glm::perspective(45.0f, (float)windowInfo.Width / (float)windowInfo.Height, 0.1f, 1000.0f)
			* glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) 
			* glm::rotate(glm::mat4(1.0f), glm::radians((float)currentTime * 19.3f), glm::vec3(0.0f, 1.0f, 0.0f)) 
			* glm::rotate(glm::mat4(1.0f), glm::radians((float)currentTime * 21.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);

		object.render();
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}
	virtual void shotdown()
	{
		glDeleteProgram(program);
		glDeleteTextures(2, tex);
	}
	virtual void  keyboard(GLFWwindow* window, int key, int scancode, int action, int mode) 
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS && key == GLFW_KEY_T)
				tex_index = !tex_index;
		}
	};
private:
	GLuint program;
	Shader TextureShader;
	Object object;
	GLuint mvp_loc, tex_loc, tex_index, tex[2];
	bool keys[1024];
};


GLuint load_texture(const char *textureFile)
{
	int width, height;
	unsigned char *image = SOIL_load_image(textureFile, &width, &height, 0, SOIL_LOAD_RGB);
	if (!image)
		std::cout << "Fail to load the texture" << std::endl;
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); 
	return texture;
}

void Texture::init_texture()
{
	GLuint texture;
	glGenTextures(1, &tex[0]);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 16, 16);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16 , 16, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	tex[1] = ktx::file::load("../media/textures/pattern1.ktx");
}


void Texture::init_shader()
{
	TextureShader.attach(GL_VERTEX_SHADER, "texture.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "texture.frag");

	TextureShader.link();
	program = TextureShader.program;
	glUseProgram(program);
	tex_loc = glGetUniformLocation(program, "tex");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	glUniform1i(tex_loc, 0);
}


void Texture::init_buffer()
{

}

void Texture::init_vertexArray()
{
}

DECLARE_MAIN(Texture);