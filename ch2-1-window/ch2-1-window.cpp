#include <GL/glew.h>
#include <sb6.h>
#include <shader.h>

class window :public sb6::Application
{
public:
	window(void):windowShader("Window") {}
	virtual ~window(void) {};
	void init_shader()
	{
		windowShader.attach(GL_VERTEX_SHADER, "window.vert");
		windowShader.attach(GL_FRAGMENT_SHADER, "window.frag");
		windowShader.link();
		program = windowShader.program;
	}
	void init_buffer()
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void init_vertexArray()
	{

	}

	virtual void init()
	{
       init_shader();
	}

	virtual void initWindowInfo()
	{
		windowInfo.title = "Window";
	}

	virtual void render()  //绘制
	{
		//static const GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f };  //清除指定的缓存
		//glClearBufferfv(GL_COLOR, 0, red);
	    float time = static_cast<float>( glfwGetTime() );
		const GLfloat color[] = { sin(time) * 0.5f + 0.5f, 
			                      cos(time) * 0.5f + 0.5f,
			                      0.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, color);
		glUseProgram(program);
	}

	virtual void shutdown()
	{
		glDeleteProgram(program);
	}

private:
	GLuint program, vao, vbo;
	Shader windowShader;
};

DECLARE_MAIN(window);  //调用主函数