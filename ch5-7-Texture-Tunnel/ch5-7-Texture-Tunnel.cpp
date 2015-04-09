#include <gl/glew.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <object.cpp>
#include <ktx.cpp>

class Texture :public sb6::Application
{
public:
	Texture():TextureShader("Texture shader"), program(0){
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
		textures[0] = tex_wall; textures[1] = tex_floor; //墙，地板，墙，天花板
		textures[2] = tex_wall; textures[3] = tex_ceiling;
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	virtual void render()
	{
		static const GLfloat gray[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		static const GLfloat ones[] = { 1.0f };
		glClearBufferfv(GL_COLOR, 0, gray);
		glClearBufferfv(GL_DEPTH, 0, ones);
		static glm::mat4 mat = glm::mat4(1.0);
		float currentTime = glfwGetTime();
		glUniform1f(offset_loc, currentTime * 0.003f);
	    for (int i = 0; i != 4; ++i)
	    {
		    glUseProgram(program);
		    glm::mat4 mvp = glm::perspective(60.0f, (float)windowInfo.Width / (float)windowInfo.Height, 0.1f, 100.0f)
			    * glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -10.0), glm::vec3(0.0, 1.0, 0.0))
			    * glm::translate(mat, glm::vec3(0.0f, 0.0f, -2.0f)) 
				* glm::rotate(mat, 90.0f * (float)i, glm::vec3(0.0f, 0.0f, 1.0f)) 
			    * glm::rotate(mat, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) 
			    * glm::scale(mat, glm::vec3(30.0f, 1.0f, 1.0f));
		   glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &mvp[0][0]);
		   glBindTexture(GL_TEXTURE_2D, textures[i]);
		   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	     }
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}
private:
	GLuint program;
	Shader TextureShader;
	GLuint mvp_loc, tex_loc, offset_loc;
	GLuint tex_wall, tex_ceiling, tex_floor;
	bool keys[1024];
	GLuint textures[4];
};

void Texture::init_texture()
{
	tex_wall = ktx::file::load("../media/textures/brick.ktx");
    tex_ceiling = ktx::file::load("../media/textures/ceiling.ktx");
	tex_floor = ktx::file::load("../media/textures/floor.ktx");
	GLuint texture[] = {tex_floor, tex_wall, tex_ceiling};
	for (int i = 0; i != 3; ++i)
	{  
      glBindTexture(GL_TEXTURE_2D, texture[i]);    //设置当前纹理的参数
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}


void Texture::init_shader()
{
	TextureShader.attach(GL_VERTEX_SHADER, "tunnel.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "tunnel.frag");

	TextureShader.link();
	program = TextureShader.program;
	glUseProgram(program);
	tex_loc = glGetUniformLocation(program, "tex");
	offset_loc = glGetUniformLocation(program, "offset");
	mvp_loc = glGetUniformLocation(program, "mvp");
	glUniform1i(tex_loc, 0);
}


void Texture::init_buffer()
{

}

void Texture::init_vertexArray()
{
}

DECLARE_MAIN(Texture);