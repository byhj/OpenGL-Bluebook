#include "Plane.h"
#include "ogl/vmath.h"

#include <glfw/glfw3.h>

enum BUFFER_TYPE_t
{
	POSITION_A,
	POSITION_B,
	VELOCITY_A,
	VELOCITY_B,
	CONNECTION
};

enum
{
	POINTS_X            = 50,
	POINTS_Y            = 50,
	POINTS_TOTAL        = (POINTS_X * POINTS_Y),
	CONNECTIONS_TOTAL   = (POINTS_X - 1) * POINTS_Y + (POINTS_Y - 1) * POINTS_X
};

namespace byhj
{

Plane::Plane()
{

}

Plane::~Plane()
{

}

void Plane::Init()
{
	glEnable(GL_DEPTH_TEST);

	init_buffer();
	init_vertexArray();
	init_shader();
}

void Plane::Render(GLfloat aspect)
{

	glUseProgram(update_program);

	//Turn off the Rasterization
	glEnable(GL_RASTERIZER_DISCARD);

	for (int i = iterations_per_frame; i != 0; --i)
	{
		glBindVertexArray(m_vao[m_iteration_index & 1]);

		glBindTexture(GL_TEXTURE_BUFFER, m_pos_tbo[m_iteration_index & 1]);
		m_iteration_index++;          //load the data to transform feedback

									  //index                buffer
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[POSITION_A + (m_iteration_index & 1)]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_vbo[VELOCITY_A + (m_iteration_index & 1)]);

		//Get the Transform Feedback data to buffer
		glBeginTransformFeedback(GL_POINTS);
		glDrawArrays(GL_POINTS, 0, POINTS_TOTAL);
		glEndTransformFeedback();

		glBindVertexArray(0);
	}

	glUseProgram(0);

	//N frame to N+1 frame
	
	//Now, we use the transform feedback data to render the scene

	glDisable(GL_RASTERIZER_DISCARD);
	//glViewport(0, 0, GetScreenWidth(), GetScreenHeight());
	static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);

	glUseProgram(render_program);

	glBindVertexArray(m_vao[1]);

	if (draw_points)
	{
		glPointSize(4.0f);
		glDrawArrays(GL_POINTS, 0, POINTS_TOTAL);
	}

	if (draw_lines)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
		glDrawElements(GL_LINES, CONNECTIONS_TOTAL * 2, GL_UNSIGNED_INT, NULL);
	}
	glBindVertexArray(0);

	glUseProgram(0);
}


void Plane::Shutdown()
{
	glDeleteProgram(update_program);
	glDeleteBuffers(5, m_vbo);
	glDeleteVertexArrays(2, m_vao);
}

void Plane::init_buffer()
{
	glm::vec4 * initial_positions = new glm::vec4[POINTS_TOTAL];
	glm::vec3 * initial_velocities = new glm::vec3[POINTS_TOTAL];
	glm::ivec4 * connection_vectors = new glm::ivec4[POINTS_TOTAL];

	int n = 0;
	for (int j = 0; j < POINTS_Y; j++)
	{
		float fj = (float)j / (float)POINTS_Y;
		for (int i = 0; i < POINTS_X; i++)
		{
			float fi = (float)i / (float)POINTS_X;

			initial_positions[n] = glm::vec4((fi - 0.5f) * (float)POINTS_X,
				(fj - 0.5f) * (float)POINTS_Y,
				0.6f * sinf(fi) * cosf(fj),
				1.0f);
			initial_velocities[n] = glm::vec3(0.0f);
			connection_vectors[n] = glm::ivec4(-1);
			if (j != (POINTS_Y - 1))
			{
				if (i != 0)
					connection_vectors[n][0] = n - 1;
				if (j != 0)
					connection_vectors[n][1] = n - POINTS_X;
				if (i != (POINTS_X - 1))
					connection_vectors[n][2] = n + 1;
				if (j != (POINTS_Y - 1))
					connection_vectors[n][3] = n + POINTS_X;
			}
			n++;
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////
	glGenVertexArrays(2, m_vao);
	glGenBuffers(5, m_vbo);

	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(m_vao[i]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[POSITION_A + i]);
		glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(glm::vec4), initial_positions, GL_DYNAMIC_COPY);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VELOCITY_A + i]);
		glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(glm::vec3), initial_velocities, GL_DYNAMIC_COPY);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[CONNECTION]);
		glBufferData(GL_ARRAY_BUFFER, POINTS_TOTAL * sizeof(glm::ivec4), connection_vectors, GL_STATIC_DRAW);
		glVertexAttribIPointer(2, 4, GL_INT, 0, NULL);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	delete[] connection_vectors;
	connection_vectors = 0;
	delete[] initial_velocities;
	initial_velocities = 0;
	delete[] initial_positions;
	initial_positions = 0;

	////////////////////////////////////////////////////////////////////////////////////
	glGenTextures(2, m_pos_tbo);
	glBindTexture(GL_TEXTURE_BUFFER, m_pos_tbo[0]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vbo[POSITION_A]);

	glBindTexture(GL_TEXTURE_BUFFER, m_pos_tbo[1]);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_vbo[POSITION_B]);

	int lines = (POINTS_X - 1) * POINTS_Y + (POINTS_Y - 1) * POINTS_X;

	glGenBuffers(1, &m_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lines * 2 * sizeof(int), NULL, GL_STATIC_DRAW);

	int * e = (int *)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, lines * 2 * sizeof(int),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (int j = 0; j < POINTS_Y; j++)
	{
		for (int i = 0; i < POINTS_X - 1; i++)
		{
			*e++ = i + j * POINTS_X;
			*e++ = 1 + i + j * POINTS_X;
		}
	}
	for (int i = 0; i < POINTS_X; i++)
	{
		for (int j = 0; j < POINTS_Y - 1; j++)
		{
			*e++ = i + j * POINTS_X;
			*e++ = POINTS_X + i + j * POINTS_X;
		}
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);


}

void Plane::init_vertexArray()
{

}

void Plane::init_shader()
{
	updateShader.init();
	updateShader.attach(GL_VERTEX_SHADER, "update.vert");
	updateShader.link();
	update_program = updateShader.GetProgram();
	//Set transform feedback before link program
	static const char * tf_varyings[] =
	{
		"tf_position_mass",
		"tf_velocity"
	};
	glTransformFeedbackVaryings(update_program, 2, tf_varyings, GL_SEPARATE_ATTRIBS);
	glLinkProgram(update_program);



	renderShader.init();
	renderShader.attach(GL_VERTEX_SHADER, "render.vert");
	renderShader.attach(GL_FRAGMENT_SHADER, "render.frag");
	renderShader.link();
	renderShader.use();
	render_program = renderShader.GetProgram();
}

}