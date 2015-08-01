#ifndef GRASS_H
#define GRASS_H


#include "ogl/oglUtility.h"
#include "ogl/oglShader.h"

namespace byhj
{


class Grass
{
public:
	Grass() = default;
	~Grass() = default;


	void Init();
	void Render(float aspect);
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	struct UniformLoc
	{
		GLuint tex_grass_color;
		GLuint tex_grass_length;
		GLuint tex_grass_orientation;
		GLuint tex_grass_bend;
		GLuint mvp;
	};
	UniformLoc uniformLoc;

	struct TextureID
	{
		GLuint      tex_grass_color;
		GLuint      tex_grass_length;
		GLuint      tex_grass_orientation;
		GLuint      tex_grass_bend;
	};
	TextureID texID;

	GLuint vao = byhj::OGL_VALUE;
	GLuint vbo = byhj::OGL_VALUE;
	GLuint program = byhj::OGL_VALUE;
	Shader GrassShader ={ "Grass Shader" };
};

}
#endif