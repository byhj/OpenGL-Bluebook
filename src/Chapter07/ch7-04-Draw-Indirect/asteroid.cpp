#include "Asteroid.h"

namespace byhj
{

	void Asteroid::Init()
	{
		init_buffer();
		init_vertexArray();
		init_shader();
	}

	void Asteroid::Render()
	{
		glUseProgram(program);
		glBindVertexArray(vao);

		//We draw four Asteroid quad with different pos and color
		glDrawArraysAsteroidd(GL_TRIANGLE_FAN, 0, 4, 4);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void Asteroid::Shutdown()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	static const GLfloat VertexData[] =
	{
		-0.5f, -0.5f, -0.0f, 1.0f,
		0.5f, -0.5f, -0.0f, 1.0f,
		0.5f,  0.5f, -0.0f, 1.0f,
		-0.5f,  0.5f, -0.0f, 1.0f,
	};
	static const GLsizei VertexSize = sizeof(VertexData);

	static const GLfloat ColorData[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
	};
	static const GLsizei ColorSize = sizeof(ColorData);


	static const GLfloat OffsetData[] =
	{
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
		1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
	};
	static const GLsizei OffsetSize = sizeof(OffsetData);

	void Asteroid::init_buffer()
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, VertexSize + ColorSize + OffsetSize, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, VertexSize, VertexData);
		glBufferSubData(GL_ARRAY_BUFFER, VertexSize, ColorSize, ColorData);
		glBufferSubData(GL_ARRAY_BUFFER, VertexSize + ColorSize, OffsetSize, OffsetData);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Asteroid::init_vertexArray()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)VertexSize);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(VertexSize + ColorSize));

		//We set the index 1&2 send to shader each Asteroid not each position
		//You can use gl_AsteroidID to achieve the current Asteroid index
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void Asteroid::init_shader()
	{
		AsteroidShader.init();
		AsteroidShader.attach(GL_VERTEX_SHADER, "Asteroid.vert");
		AsteroidShader.attach(GL_FRAGMENT_SHADER, "Asteroid.frag");
		AsteroidShader.link();
		AsteroidShader.info();
		program = AsteroidShader.GetProgram();
	}



}