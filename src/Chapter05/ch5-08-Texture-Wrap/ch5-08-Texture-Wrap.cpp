#include <gl/glew.h>
#include <sb6/sb6.h>
#include <sb6/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sb6/object.cpp>
#include <sb6/ktx.cpp>

class Texture :public sb6::Application
{
public:
	Texture():TextureShader("Texture shader"), program(0)
	{
	}
	~Texture(){}
	void init_shader();
	void init_texture();

	void vInit()
	{
		init_shader();
		init_texture();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	void vRender()
	{
		static const GLfloat gray[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		static const GLfloat ones[] = { 1.0f };
		glClearBufferfv(GL_COLOR, 0, gray);
		glClearBufferfv(GL_DEPTH, 0, ones);
					                         //left bottom   right bottom    left top            right top
		static const GLenum wrapModes[] = { GL_CLAMP_TO_EDGE,  GL_REPEAT,  GL_CLAMP_TO_BORDER,  GL_MIRRORED_REPEAT  };
		static const GLfloat offsets[] = 
		{ 
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			-0.5f,  0.5f,
			 0.5f,  0.5f
		};

		//Set the wrap border color
		static const GLfloat blue[] = { 0.0f, 0.0f, 1.0f, 1.0f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, blue);  
		glUseProgram(program);

		for (int i = 0; i != 4; ++i)
		{
			//We move the rectangle to four corner
			glUniform2fv(glGetUniformLocation(program, "offset"), 1, &offsets[i * 2]);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModes[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModes[i]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}

	virtual void vShutdown()
	{
		glDeleteProgram(program);
		glDeleteTextures(1, &tex);
	}

private:
	GLuint program;
	Shader TextureShader;
	GLuint  tex_loc, tex;
};


void Texture::init_texture()
{
	tex = sb6::ktx::load("../../../media/textures/rightarrows.ktx");
}


void Texture::init_shader()
{
	TextureShader.attach(GL_VERTEX_SHADER, "wrap.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "wrap.frag");
	TextureShader.link();

	program = TextureShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");
	glUniform1i(tex_loc, 0);
}


DECLARE_MAIN(Texture);