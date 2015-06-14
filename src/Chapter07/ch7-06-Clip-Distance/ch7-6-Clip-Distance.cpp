#include <gl/glew.h>
#include <sb6/sb6.h>
#include <sb6/shader.h>
#include <sb6/object.cpp>
#include <sb6/vmath.h>

class Clip :public byhj::Application
{
public:
	Clip():ClipShader("Clip shader"), program(0){}
	~Clip(){}
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_Clip();

	void v_Init()
	{
		init_shader();
        init_buffer();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CLIP_DISTANCE0);
		glEnable(GL_CLIP_DISTANCE1);
	}

	virtual void v_Render()
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static const GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);
	    float f = (float)glfwGetTime();

		glUseProgram(program);
		float currTime = static_cast<float>(glfwGetTime()) / 10.0f;

		vmath::mat4 proj_matrix = vmath::perspective(50.0f, GetAspect(), 0.1f, 1000.0f);

		vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -15.0f)
			                  * vmath::rotate(f * 0.34f, 0.0f, 1.0f, 0.0f) 
							  * vmath::translate(0.0f, -4.0f, 0.0f);

		vmath::mat4 plane_matrix = vmath::rotate(f * 6.0f, 1.0f, 0.0f, 0.0f) 
			                     * vmath::rotate(f * 7.3f, 0.0f, 1.0f, 0.0f);

		vmath::vec4 plane = plane_matrix[0];
		plane[3] = 0.0f;
		plane = vmath::normalize(plane);
		vmath::vec4 clip_sphere = vmath::vec4(sinf(f * 0.7f) * 3.0f, cosf(f * 1.9f) * 3.0f,
			                                  sinf(f * 0.1f) * 3.0f, cosf(f * 1.7f) + 2.5f);

		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj_matrix);
		glUniformMatrix4fv(mv_loc, 1, GL_FALSE, mv_matrix);
		glUniform4fv(clip_plane_loc, 1, plane);
		glUniform4fv(clip_sphere_loc, 1, clip_sphere);

		object.render();
	}

	void v_Shutdown()
	{
		glDeleteProgram(program);
	}
private:

	GLuint program;
	Shader ClipShader;
	sb6::Object object;

	GLint   proj_loc;
	GLint   mv_loc;
	GLint   clip_plane_loc;
	GLint   clip_sphere_loc;
};
	
CALL_MAIN(Clip);


void Clip::init_Clip()
{
	GLuint Clip;

}


void Clip::init_shader()
{
	ClipShader.init();
	ClipShader.attach(GL_VERTEX_SHADER, "Clip.vert");
	ClipShader.attach(GL_FRAGMENT_SHADER, "Clip.frag");
	ClipShader.link();
	program = ClipShader.GetProgram();

	glUseProgram(program);
	proj_loc = glGetUniformLocation(program, "proj_matrix");
	mv_loc = glGetUniformLocation(program, "mv_matrix");
	clip_plane_loc = glGetUniformLocation(program, "clip_plane");
	clip_sphere_loc = glGetUniformLocation(program, "clip_sphere");
}

void Clip::init_buffer()
{
	object.load("../../../media/objects/dragon.sbm");
}

void Clip::init_vertexArray()
{
}

