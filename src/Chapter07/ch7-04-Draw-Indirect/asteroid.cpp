#include "Asteroid.h"
#include <GLFW/glfw3.h>

namespace byhj
{

	void Asteroid::Init()
	{
		init_buffer();
		init_vertexArray();
		init_shader();
	}

	void Asteroid::Render(GLfloat aspect)
	{
		float currentTime = glfwGetTime();
		static double last_time = 0.0;
		static double total_time = 0.0;
		total_time += (currentTime - last_time);
		last_time = currentTime;
		float t = float(total_time);
		int i = int(total_time * 3.0f);


		glm::mat4 view_matrix = glm::lookAt(glm::vec3(100.0f * cosf(t * 0.023f), 100.0f * cosf(t * 0.023f), 300.0f * sinf(t * 0.037f) - 500.0f),
			glm::vec3(0.0f, 0.0f, 260.0f),
			glm::normalize(glm::vec3(0.1f - cosf(t * 0.1f) * 0.3f, 1.0f, 0.0f)));

		glm::mat4 proj_matrix = glm::perspective(45.0f, aspect, 1.0f, 2000.0f);

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

		glUseProgram(0);
	}

	void Asteroid::Shutdown()
	{
		glDeleteProgram(program);
	}


	void Asteroid::init_buffer()
	{
		object.load("../../../media/objects/asteroids.sbm");

		//We load the command use vertex buffer 
		glGenBuffers(1, &indirect_vbo);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_vbo);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, NUM_DRAWS * sizeof(DrawArraysIndirectCommand), NULL, GL_STATIC_DRAW);

		//We map the data the buffer
		DrawArraysIndirectCommand *cmd = (DrawArraysIndirectCommand *)glMapBufferRange(GL_DRAW_INDIRECT_BUFFER,
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


	void Asteroid::init_shader()
	{
		AsteroidShader.init();
		AsteroidShader.attach(GL_VERTEX_SHADER, "asteroid.vert");
		AsteroidShader.attach(GL_FRAGMENT_SHADER, "asteroid.frag");
		AsteroidShader.link();
		program = AsteroidShader.GetProgram();

		view_loc     = glGetUniformLocation(program, "view_matrix");
		proj_loc     = glGetUniformLocation(program, "proj_matrix");
		viewproj_loc = glGetUniformLocation(program, "viewproj_matrix");
		time_loc     = glGetUniformLocation(program, "time");
	}



}