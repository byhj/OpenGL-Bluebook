#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/object.h"

namespace byhj
{

class Geometry
{
public:
	Geometry();
	~Geometry();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	GLuint fan_prog;
	GLuint line_prog;

	GLuint fan_mvp_loc;
	GLuint fan_vid_offset_loc;
	GLuint line_mvp_loc;
	GLuint line_vid_offset_loc;

	Shader FanShader;
	Shader LineShader;

	int         mode = 0;
	int         vid_offset = 0;
	bool        paused = false;
};

}
#endif
