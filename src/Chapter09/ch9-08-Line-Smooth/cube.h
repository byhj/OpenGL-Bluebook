#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/object.h"

namespace byhj
{

class Cube
{
public:
	Cube();
	~Cube();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void render_scene(double currentTime);

	GLuint VBO, IBO;
	GLuint VAO, program;
	ogl::Shader cubeShader;
	GLuint mv_location, proj_location;;

};

}
#endif
