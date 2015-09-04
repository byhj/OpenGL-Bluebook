#ifndef Plane_H
#define Plane_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/vmath.h"
#include "ogl/object.h"

#define DEPTH_TEXTURE_SIZE      4096
#define FRUSTUM_DEPTH           1000
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
	void render_scene(double currentTime, bool from_light);

	GLuint          light_program;
	GLuint          view_program;
	GLint           show_light_depth_program;
	ogl::Shader LightShader;
	ogl::Shader ViewShader;
	ogl::Shader ShowShader;
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
		} view;
	} uniforms;

	GLuint          depth_fbo;
	GLuint          depth_tex;
	GLuint          depth_debug_tex;

	enum { OBJECT_COUNT = 4 };
	struct
	{
		sb6::Object     obj;
		vmath::mat4     model_matrix;
	} objects[OBJECT_COUNT];

	vmath::mat4     light_view_matrix;
	vmath::mat4     light_proj_matrix;

	vmath::mat4     camera_view_matrix;
	vmath::mat4     camera_proj_matrix;

	GLuint          quad_vao;

	enum
	{
		RENDER_FULL,
		RENDER_LIGHT,
		RENDER_DEPTH
	} mode;

	bool paused;
};

}
#endif
