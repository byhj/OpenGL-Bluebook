#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
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
	void init_texture();
	GLuint      gbuffer;
	GLuint      gbuffer_tex[3];
	GLuint      fs_quad_vao;

	sb6::Object object;

	Shader RenderShader;
	Shader RenderNMShader;
	Shader LightShader;
	Shader VisShader;
	GLuint      render_program;
	GLuint      render_program_nm;
	GLuint      render_transform_ubo;

	GLuint      light_program;
	GLuint      light_ubo;

	GLuint      vis_program;
	GLint       loc_vis_mode;

	GLuint      tex_diffuse;
	GLuint      tex_nm;

	bool        use_nm = true;
	bool        paused = false;

	enum
	{
		VIS_OFF,
		VIS_NORMALS,
		VIS_WS_COORDS,
		VIS_DIFFUSE,
		VIS_META
	} vis_mode = VIS_OFF;

#pragma pack (push, 1)
	struct light_t
	{
		vmath::vec3         position;
		unsigned int : 32;       // pad0
		vmath::vec3         color;
		unsigned int : 32;       // pad1
	};
#pragma pack (pop)

};

}
#endif
