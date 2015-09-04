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
	GLuint  view_program;
	GLint   show_light_depth_program;

	struct
	{
		struct
		{
			GLint   mvp;
		} light;
		struct
		{
			GLint   mv_matrix;
			GLint   proj_matrix;
			GLint   shadow_matrix;
			GLint   full_shading;
			GLint   specular_albedo;
			GLint   diffuse_albedo;
		} view;
	} uniforms;

	enum { OBJECT_COUNT = 4 };
	struct
	{
		sb6::Object     obj;
		glm::mat4  model_matrix;
	} objects[OBJECT_COUNT];

	glm::mat4     light_view_matrix;
	glm::mat4     light_proj_matrix;

	glm::mat4     camera_view_matrix[2];
	glm::mat4     camera_proj_matrix;

	GLuint          quad_vao;

	float           separation = 2.0f;

	enum
	{
		RENDER_FULL,
		RENDER_LIGHT,
		RENDER_DEPTH
	} mode = RENDER_FULL;

	bool paused;
	ogl::Shader StereoShader;
};

}
#endif
