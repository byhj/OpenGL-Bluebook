#include <gl/glew.h>
#include "ogl/shader.h"
#include "ogl/oglApp.h"

class Subroutine: public byhj::Application
{
public:
	Subroutine():program(0), subShader("Subroutine Shader") {};
	~Subroutine(){}
public:
	void v_Init();
	void v_Render();
	void init_shader();

private:
	GLuint program, sub_loc;
	GLuint subroutines[2];
	Shader subShader;
};

void Subroutine::v_Init()
{
	init_shader();
}

void Subroutine::v_Render()
{
	glUseProgram(program);

	int i = static_cast<int>( glfwGetTime() );  

	//Notice: You should call this func after glUseProgram, we change subFunc every second
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutines[i & 1]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(0);
}

void Subroutine::init_shader()
{
	subShader.init();
	subShader.attach(GL_VERTEX_SHADER, "subroutine.vert");
	subShader.attach(GL_FRAGMENT_SHADER, "subroutine.frag");
	subShader.link();
	program = subShader.GetProgram();

    //Get the subFunc index in fragment shader
	subroutines[0] = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "myFunction1");  
	subroutines[1] = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "myFunction2");
	sub_loc = glGetSubroutineUniformLocation(program, GL_FRAGMENT_SHADER, "subroutineUniform");
}

CALL_MAIN(Subroutine);
