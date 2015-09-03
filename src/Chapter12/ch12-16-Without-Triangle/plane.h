#ifndef Plane_H
#define Plane_H

#include <GL/glew.h>
#include "ogl/Shader.h"

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
	void init_texture();

	GLuint      program;
	GLuint      vao;
	GLuint      palette_texture;

	Shader JuliaShader;
	struct
	{
		GLint   zoom;
		GLint   offset;
		GLint   C;
	} uniforms;

	bool paused = false;
	float time_offset = 0.0f;
	float x_offset    = 0.0f;
	float y_offset    = 0.0f;
	float zoom = 1.0f;
};

}
#endif
