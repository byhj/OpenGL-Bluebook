#ifndef Plane_H
#define Plane_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/object.h"


namespace byhj
{

class Plane
{
public:
	Plane();
	~Plane();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	Shader updateShader = { "update Shader" };
	Shader renderShader = { "render Shader" };
	GLuint m_vao[2];
	GLuint m_vbo[5];
	GLuint m_index_buffer;
	GLuint m_pos_tbo[2];
	GLuint update_program;
	GLuint render_program;
	GLuint m_C_loc;
	GLuint m_iteration_index = 0;
	bool   draw_points = false;
	bool   draw_lines  = true;
	int    iterations_per_frame = 16;
};

}
#endif
