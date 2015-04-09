#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

char *vertName[3] = {"triangle.vert", "quad.vert", "isoline.vert" };
char *tcsName[3]  =  {"triangle.tcs", "quad.tcs", "isoline.tcs" };
char *tesName[3]  =  {"triangle.tes", "quad.tes", "isoline.tes" };
char *fragName[3] =  {"triangle.frag", "quad.frag", "isoline.frag" };

class IndexCube: public sb6::Application
{
public:
	IndexCube():program_index(2) {};
	~IndexCube(){}
	virtual void init();
	virtual void render();
	void init_shader();
public:
	GLuint  program[3], vao;
	GLuint  program_index;
	Shader TessellationShader[3];
};

void IndexCube::init()
{
	init_shader();
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void IndexCube::render()
{
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, green);
	glUseProgram(program[program_index]);
	glDrawArrays(GL_PATCHES, 0, 4);

}

void IndexCube::init_shader()
{
	for (int i = 0; i != 3 ; ++i) 
	{
		TessellationShader[i].init();
		TessellationShader[i].attach(GL_VERTEX_SHADER, vertName[i]);
		TessellationShader[i].attach(GL_TESS_CONTROL_SHADER, tcsName[i]);
		TessellationShader[i].attach(GL_TESS_EVALUATION_SHADER, tesName[i]);
		TessellationShader[i].attach(GL_FRAGMENT_SHADER, fragName[i]);
		TessellationShader[i].link();
		program[i] = TessellationShader[i].program;
	}
}

DECLARE_MAIN(IndexCube);


