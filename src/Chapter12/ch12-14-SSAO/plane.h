#ifndef Plane_H
#define Plane_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/object.h"
#include "ogl/vmath.h"

namespace byhj
{

class Plane
{
public:
	Plane() : render_program(0),
		ssao_program(0),
		paused(false),
		ssao_level(1.0f),
		ssao_radius(0.05f),
		show_shading(true),
		show_ao(true),
		weight_by_angle(true),
		randomize_points(true),
		point_count(10) {}

	~Plane();

	void Init();
	void Render(GLfloat aspect, int sw, int sh);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	Shader RenderShader;
	Shader SsaoShader;
	GLuint      render_program;
	GLuint      ssao_program;
	bool        paused;

	GLuint      render_fbo;
	GLuint      fbo_textures[3];
	GLuint      quad_vao;
	GLuint      points_buffer;

	sb6::Object object;
	sb6::Object cube;

	struct
	{
		struct
		{
			GLint           mv_matrix;
			GLint           proj_matrix;
			GLint           shading_level;
		} render;
		struct
		{
			GLint           ssao_level;
			GLint           object_level;
			GLint           ssao_radius;
			GLint           weight_by_angle;
			GLint           randomize_points;
			GLint           point_count;
		} ssao;
	} uniforms;

	bool  show_shading;
	bool  show_ao;
	float ssao_level;
	float ssao_radius;
	bool  weight_by_angle;
	bool randomize_points;
	unsigned int point_count;

	struct SAMPLE_POINTS
	{
		vmath::vec4     point[256];
		vmath::vec4     random_vectors[256];
	};
};

}
#endif
