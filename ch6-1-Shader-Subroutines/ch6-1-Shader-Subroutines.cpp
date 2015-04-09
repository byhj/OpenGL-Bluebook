#include <gl/glew.h>
#include <gl/glfw3.h>
#include <shader.h>
#include <sb6.h>

class Subroutine: public sb6::Application
{
public:
	Subroutine():program(0), subShader("Subroutine") {};
	~Subroutine(){}
public:
	virtual void init();
	virtual void render();
	void init_shader();

private:
	GLuint program, sub_loc;
	GLuint subroutines[2];
	Shader subShader;
};

void Subroutine::init()
{
	init_shader();
}

void Subroutine::render()
{
	int i = static_cast<int>( glfwGetTime() );  //多少秒， 一秒变换一次
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutines[i & 1]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);x
}

void Subroutine::init_shader()
{
	subShader.attach(GL_VERTEX_SHADER, "subroutine.vert");
	subShader.attach(GL_FRAGMENT_SHADER, "subroutine.frag");
	subShader.link();
	subShader.use();
	program = subShader.program;
	subroutines[0] = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "myFunction1");   //获取
	subroutines[1] = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "myFunction2");
	sub_loc = glGetSubroutineUniformLocation(program, GL_FRAGMENT_SHADER, "subroutineUniform");
}

DECLARE_MAIN(Subroutine);