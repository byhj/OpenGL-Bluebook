#ifndef GRASS_H
#define GRASS_H


#include "ogl/Utility.h"
#include "ogl/Shader.h"

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

	GLuint vao = ogl::VALUE;
	GLuint vbo = ogl::VALUE;
	GLuint program = ogl::VALUE;
	ogl::Shader GrassShader ={ "Grass Shader" };
};

}
#endif