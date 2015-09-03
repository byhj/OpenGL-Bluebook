#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/object.h"

namespace byhj
{

class Sphere
{
public:
	Sphere();
	~Sphere();

	void Init();
	void Render(GLfloat aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void render_scene(double currentTime);

	GLuint program, mvp_loc, mv_loc;
	Shader RimLightShader;
	GLint           rim_color_loc;
	GLint           rim_power_loc;

	glm::mat4  mat_rotation;
	sb6::Object      object;
	bool        paused;
	glm::vec3   rim_color = glm::vec3(0.3f, 0.3f, 0.3f);
	float       rim_power = 2.5f;
	bool        rim_enable = true;

};

}
#endif
