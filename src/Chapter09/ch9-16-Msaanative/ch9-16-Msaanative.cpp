#include <sb6/shader.h>
#include <sb6/sb6.h>
#include <sb6/vmath.h>

class msaanative_app : public byhj::Application
{
    void v_Init()
    {
		init_buffer();
		init_shader();
		glEnable(GL_CULL_FACE);
    }

   void v_Render()
    {
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        static const GLfloat one = 1.0f;

        glViewport(0, 0, GetScreenWidth(), GetScreenHeight());
        glClearBufferfv(GL_COLOR, 0, black);

        glUseProgram(program);
		glBindVertexArray(vao);
        vmath::mat4 proj_matrix = vmath::perspective(50.0f, GetAspect(),  0.1f, 1000.0f);
        glUniformMatrix4fv(proj_location, 1, GL_FALSE, proj_matrix);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		float currentTime = glfwGetTime();
#ifdef MANY_CUBES
        for (i = 0; i < 24; i++)
        {
            float f = (float)i + (float)currentTime * 0.3f;
            vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -20.0f) *
                                    vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
                                    vmath::rotate((float)currentTime * 21.0f, 1.0f, 0.0f, 0.0f) *
                                    vmath::translate(sinf(2.1f * f) * 2.0f,
                                                     cosf(1.7f * f) * 2.0f,
                                                     sinf(1.3f * f) * cosf(1.5f * f) * 2.0f);
            glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv_matrix);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
        }
#else
        float f = (float)currentTime * 0.3f;
        currentTime = 3.15;
        vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -4.0f) *
                                /*vmath::translate(sinf(2.1f * f) * 0.5f,
                                                    cosf(1.7f * f) * 0.5f,
                                                    sinf(1.3f * f) * cosf(1.5f * f) * 2.0f) **/
                                vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
                                vmath::rotate((float)currentTime * 81.0f, 1.0f, 0.0f, 0.0f);
        glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv_matrix);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
#endif
    }

    void v_Shutdown()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(program);
        glDeleteBuffers(1, &position_buffer);
    }

private:
	void init_buffer();
	void init_shader();

	Shader MsaaShader;
    GLuint          program;
    GLuint          vao;
    GLuint          position_buffer;
    GLuint          index_buffer;
    GLint           mv_location;
    GLint           proj_location;
	int samples;
};

CALL_MAIN(msaanative_app)
	static const GLushort vertex_indices[] =
{
	0, 1, 2,
	2, 1, 3,
	2, 3, 4,
	4, 3, 5,
	4, 5, 6,
	6, 5, 7,
	6, 7, 0,
	0, 7, 1,
	6, 0, 2,
	2, 4, 6,
	7, 5, 3,
	7, 3, 1
};

static const GLfloat vertex_positions[] =
{
	-0.25f, -0.25f, -0.25f,
	-0.25f,  0.25f, -0.25f,
	0.25f, -0.25f, -0.25f,
	0.25f,  0.25f, -0.25f,
	0.25f, -0.25f,  0.25f,
	0.25f,  0.25f,  0.25f,
	-0.25f, -0.25f,  0.25f,
	-0.25f,  0.25f,  0.25f,
};


void msaanative_app::init_buffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
}

void msaanative_app::init_shader()
{	
	MsaaShader.init();
	MsaaShader.attach(GL_VERTEX_SHADER, "msaa.vert");
	MsaaShader.attach(GL_FRAGMENT_SHADER, "msaa.frag");
	MsaaShader.link();
	program = MsaaShader.GetProgram();
	mv_location = glGetUniformLocation(program, "mv_matrix");
	proj_location = glGetUniformLocation(program, "proj_matrix");

}