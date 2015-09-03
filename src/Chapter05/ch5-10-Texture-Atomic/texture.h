#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include <array>

#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/object.h"
#include "ogl/vmath.h"

constexpr int TEX_NUM = 256;

namespace byhj
{

class Texture
{
public:
	Texture();
	~Texture();

	void Init(int sw, int sh);
	void Render(float aspect);
	void Shutdown();

private:
	void update();
	void init_buffer(int sw, int sh);
	void init_vertexArray();
	void init_shader();
	void init_texture();

	Shader TextureArrayShader = { "TextureArray Shader" };

	GLuint clear_program   = ogl::VALUE;
	GLuint append_program  = ogl::VALUE;
	GLuint resolve_program = ogl::VALUE;

	Shader  ClearShader    = {"Clear Shader"};
	Shader  AppendShader   = { "Append Shader" };
	Shader  ResolveShader  = { "Resovle Shader" };

	struct
	{
		GLuint      color;
		GLuint      normals;
	} textures;

	struct uniforms_block
	{
		vmath::mat4     mv_matrix;
		vmath::mat4     view_matrix;
		vmath::mat4     proj_matrix;
	};

	GLuint          uniforms_buffer;

	struct
	{
		GLint           mvp;
	} uniforms;

	sb6::Object     object;

	GLuint          fragment_buffer;
	GLuint          head_pointer_image;
	GLuint          atomic_counter_buffer;
	GLuint          dummy_vao;
};

}
#endif
