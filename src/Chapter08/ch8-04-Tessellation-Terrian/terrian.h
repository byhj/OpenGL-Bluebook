#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"

namespace byhj
{

class Terrian
{
public:
	Terrian();
	~Terrian();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	GLuint          program;
	GLuint          vao;
	GLuint          tex_displacement;
	GLuint          tex_color;
	float           dmap_depth = 0.0f;
	bool            enable_displacement = true;
	bool            wireframe = true;
	bool            enable_fog = true;
	bool            paused;

	ogl::Shader dispmapShader;
	struct
	{
		GLint       mvp_matrix;
		GLint       mv_matrix;
		GLint       proj_matrix;
		GLint       dmap_depth;
		GLint       enable_fog;
	} uniforms;

};

}
#endif
