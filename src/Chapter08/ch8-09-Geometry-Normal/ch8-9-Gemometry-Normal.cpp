#include <gl/glew.h>
#include <sb6/shader.h>
#include <sb6/sb6.h>
#include <sb6/object.cpp>
#include <sb6/vmath.h>

class NormalApp: public byhj::Application
{
public:
	NormalApp(){};
	virtual ~NormalApp(){};

public:
	void v_Init();
	void v_Render();
	void init_buffer();
	void init_shader();

private:
	GLuint program;
	GLuint mv_loc, mvp_loc, explode_factor_loc;
	Shader CullingShader;
	sb6::Object object;
};

CALL_MAIN(NormalApp);

void NormalApp::init_shader()
{
	CullingShader.attach(GL_VERTEX_SHADER,   "normal.vert");
	CullingShader.attach(GL_GEOMETRY_SHADER, "normal.geom");
	CullingShader.attach(GL_FRAGMENT_SHADER, "normal.frag");
	CullingShader.link();
	program = CullingShader.GetProgram();

    mv_loc = glGetUniformLocation(program, "mv_matrix");
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
	explode_factor_loc = glGetUniformLocation(program, "explode_factor");
}

void NormalApp::v_Init()
{
	init_shader();
	object.load("../../../media/objects/bunny_1k.sbm");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void NormalApp::v_Render()
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;

	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glUseProgram(program);
	float f = glfwGetTime();

	vmath::mat4 proj_matrix = vmath::perspective(50.0f, GetAspect(), 0.1f, 1000.0f);

	vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -1.0f) 
		                  * vmath::rotate((float)f * 45.0f, 0.0f, 1.0f, 0.0f) 
		                  * vmath::rotate((float)f * 81.0f, 1.0f, 0.0f, 0.0f);
	
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, proj_matrix * mv_matrix);
	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, mv_matrix);

	object.render();
}