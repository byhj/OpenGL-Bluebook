#ifndef Triangle_H
#define Triangle_H

#include <GL/glew.h>
#include "ogl/Shader.h"
#include "ogl/Utility.h"

namespace byhj
{

class Texture
{
public:
	Texture();
	~Texture();

	void Init();
	void Render();
	void Shutdown();

private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	void generate_texture(float *data, int width, int height);
	void init_texture();

	Shader TextureShader = { "Texture Shader" };
	GLuint program = ogl::VALUE;
	GLuint tex_loc = ogl::VALUE;
	GLuint texture = ogl::VALUE;
};

}
#endif
