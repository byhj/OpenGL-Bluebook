#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/object.h"
#include "ogl/vmath.h"

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

	GLuint          prepare_program;
	GLuint          trace_program;
	GLuint          blit_program;

	ogl::Shader PrepareShader;
	ogl::Shader RaytracerShader;
	ogl::Shader BlitShader;

	struct uniforms_block
	{
		vmath::mat4     mv_matrix;
		vmath::mat4     view_matrix;
		vmath::mat4     proj_matrix;
	};

	GLuint          uniforms_buffer;
	GLuint          sphere_buffer;
	GLuint          plane_buffer;
	GLuint          light_buffer;

	struct
	{
		GLint           ray_origin;
		GLint           ray_lookat;
		GLint           aspect;
	} uniforms;

	GLuint          vao;

	struct sphere
	{
		vmath::vec3     center;
		float           radius;
		// unsigned int    : 32; // pad
		vmath::vec4     color;
	};

	struct plane
	{
		vmath::vec3     normal;
		float           d;
	};

	struct light
	{
		vmath::vec3     position;
		unsigned int : 32;       // pad
	};

	enum
	{
		MAX_RECURSION_DEPTH     = 5,
		MAX_FB_WIDTH            = 2048,
		MAX_FB_HEIGHT           = 1024
	};

	enum DEBUG_MODE
	{
		DEBUG_NONE,
		DEBUG_REFLECTED,
		DEBUG_REFRACTED,
		DEBUG_REFLECTED_COLOR,
		DEBUG_REFRACTED_COLOR
	};

	GLuint              tex_composite;
	GLuint              ray_fbo[MAX_RECURSION_DEPTH];
	GLuint              tex_position[MAX_RECURSION_DEPTH];
	GLuint              tex_reflected[MAX_RECURSION_DEPTH];
	GLuint              tex_reflection_intensity[MAX_RECURSION_DEPTH];
	GLuint              tex_refracted[MAX_RECURSION_DEPTH];
	GLuint              tex_refraction_intensity[MAX_RECURSION_DEPTH];

	int                 max_depth = 1;
	int                 debug_depth = 0;
	DEBUG_MODE          debug_mode = DEBUG_NONE;
	bool                paused = false;

	void                recurse(int depth);

};

}
#endif
