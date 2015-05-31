#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sb6/sb6.h>
#include <sb6/shader.h>
#include <sb6/object.cpp>
#include <sb6/ktx.cpp>
#include <sb6/vmath.h>


class Texture :public sb6::Application
{
public:
	Texture():TextureShader("Texture shader"), program(0)
	{
		memset(keys, 0, sizeof(keys));
	}

	~Texture(){}
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();

   void vInit()
	{
		init_shader();
		init_texture();

		textures[0] = tex_wall; textures[1] = tex_floor; //墙，地板，墙，天花板
		textures[2] = tex_wall; textures[3] = tex_ceiling;

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
	}

	void vRender()
	{
		static const GLfloat gray[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		static const GLfloat ones[] = { 1.0f };
		glClearBufferfv(GL_COLOR, 0, gray);
		glClearBufferfv(GL_DEPTH, 0, ones);
		float currentTime = glfwGetTime();

		glUniform1f(offset_loc, currentTime * 0.003f);
		glUseProgram(program);

		vmath::mat4 proj = vmath::perspective(60.0f,
			(float)windowInfo.Width / (float)windowInfo.Height,
			0.1f, 100.0f);

		for (int i = 0; i != 4; ++i)
		{

			vmath::mat4 mv = vmath::rotate(90.0f * (float)i, vmath::vec3(0.0f, 0.0f, 1.0f)) *
				vmath::translate(-0.5f, 0.0f, -10.0f) *
				vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f) *
				vmath::scale(30.0f, 1.0f, 1.0f);
			vmath::mat4 mvp = proj * mv;

			glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, mvp);

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

DECLARE_MAIN(Texture);

void Texture::init_texture()
{
	tex_wall    = sb6::ktx::load("../../../media/textures/brick.ktx");
	tex_ceiling = sb6::ktx::load("../../../media/textures/ceiling.ktx");
	tex_floor   = sb6::ktx::load("../../../media/textures/floor.ktx");

	GLuint texture[] = {tex_floor, tex_wall, tex_ceiling};

	for (int i = 0; i != 3; ++i)
	{  
		glBindTexture(GL_TEXTURE_2D, texture[i]);    //设置当前纹理的参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::init_shader()
{
	TextureShader.init();
	TextureShader.attach(GL_VERTEX_SHADER, "tunnel.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "tunnel.frag");
	TextureShader.link();
	program = TextureShader.GetProgram();

	glUseProgram(program);
	tex_loc = glGetUniformLocation(program, "tex");
	offset_loc = glGetUniformLocation(program, "offset");
	mvp_loc = glGetUniformLocation(program, "mvp");
	glUniform1i(tex_loc, 0);
}