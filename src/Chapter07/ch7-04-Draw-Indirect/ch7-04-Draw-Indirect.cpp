#include <gl/glew.h>
#include "ogl/oglApp.h"
#include "ogl/shader.h"
#include "ogl/object.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/ktx.cpp"

enum {
	NUM_DRAWS = 50000
};

//The draw command struct for indirect draw
struct DrawArraysIndirectCommand 
{
	GLuint count;
	GLuint primCount;
	GLuint first;
	GLuint baseInstance;
};


class Asteroid :public byhj::Application
{
public:
	Asteroid():AsteroidShader("Asteroid shader"), program(0), mode(MODE_MULTIDRAW){}
	~Asteroid(){}

	void v_Init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();


	}

	void v_Render()
	{
		float currentTime = glfwGetTime();
		static double last_time = 0.0;
		static double total_time = 0.0;
		total_time += (currentTime - last_time);
		last_time = currentTime;
		float t = float(total_time);
		int i = int(total_time * 3.0f);

		//Clear the frame buffer
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);

		glm::mat4 view_matrix = glm::lookAt(glm::vec3(100.0f * cosf(t * 0.023f), 100.0f * cosf(t * 0.023f), 300.0f * sinf(t * 0.037f) - 500.0f),
			                                      glm::vec3(0.0f, 0.0f, 260.0f),
			                                      glm::normalize(glm::vec3(0.1f - cosf(t * 0.1f) * 0.3f, 1.0f, 0.0f)));

		glm::mat4 proj_matrix = glm::perspective(45.0f, GetAspect(), 1.0f, 2000.0f);

		glUseProgram(program);

		glUniform1f(time_loc, t);
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view_matrix[0][0]);
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &proj_matrix[0][0]);
		glUniformMatrix4fv(viewproj_loc, 1, GL_FALSE, &(proj_matrix * view_matrix)[0][0]);

		glBindVertexArray(object.get_vao());
		if (mode == MODE_MULTIDRAW)
		{
			glMultiDrawArraysIndirect(GL_TRIANGLES, NULL, NUM_DRAWS, 0);
		}
		else if (mode == MODE_SEPARATE_DRAWS)
		{
			for (int j = 0; j < NUM_DRAWS; j++)
			{
				GLuint first, count;
				object.get_sub_object_info(j % object.get_sub_object_count(), first, count);
				glDrawArraysInstancedBaseInstance(GL_TRIANGLES, first, count, 1, j);
			}
		}

	}

	void v_Shutdown()
	{
		glDeleteProgram(program);
	}
	enum MODE
	{
		MODE_FIRST,
		MODE_MULTIDRAW = 0,
		MODE_SEPARATE_DRAWS,
		MODE_MAX = MODE_SEPARATE_DRAWS
	};

	MODE                mode;
private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();

private:


};
CALL_MAIN(Asteroid);

void Asteroid::init_shader()
{
	AsteroidShader.init();
	AsteroidShader.attach(GL_VERTEX_SHADER, "asteroid.vert");
	AsteroidShader.attach(GL_FRAGMENT_SHADER, "asteroid.frag");
	AsteroidShader.link();
	program = AsteroidShader.GetProgram();
	glUseProgram(program);

	view_loc = glGetUniformLocation(program, "view_matrix");
	proj_loc = glGetUniformLocation(program, "proj_matrix");
	viewproj_loc = glGetUniformLocation(program, "viewproj_matrix");
	time_loc = glGetUniformLocation(program, "time");
}


void Asteroid::init_buffer()
{
	object.load("../../../media/objects/asteroids.sbm");

    //We load the command use vertex buffer 
	glGenBuffers(1, &indirect_vbo);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_vbo);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, NUM_DRAWS * sizeof(DrawArraysIndirectCommand), NULL, GL_STATIC_DRAW);

	//We map the data the buffer
	DrawArraysIndirectCommand *cmd = (DrawArraysIndirectCommand *) glMapBufferRange( GL_DRAW_INDIRECT_BUFFER, 
		                              0, NUM_DRAWS * sizeof(DrawArraysIndirectCommand),
		                              GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	    for (int i = 0; i < NUM_DRAWS; i++)
	    {  
	    	object.get_sub_object_info(i % object.get_sub_object_count(),
	    		                       cmd[i].first,
	    		                       cmd[i].count);
	    	cmd[i].primCount = 1;
	    	cmd[i].baseInstance = i;
	    }
	glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);

	glBindVertexArray(object.get_vao());
	glGenBuffers(1, &draw_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, draw_vbo);
	glBufferData(GL_ARRAY_BUFFER, NUM_DRAWS * sizeof(GLuint), NULL, GL_STATIC_DRAW);

	GLuint * draw_index = (GLuint *)glMapBufferRange(GL_ARRAY_BUFFER,
		                            0, NUM_DRAWS * sizeof(GLuint),
		                            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (int i = 0; i < NUM_DRAWS; i++)
	{
		draw_index[i] = i;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribIPointer(10, 1, GL_UNSIGNED_INT, 0, NULL);
	glVertexAttribDivisor(10, 1);
	glEnableVertexAttribArray(10);

}

void Asteroid::init_vertexArray()
{	
}

