#include "Plane.h"
#include "ogl/ktx.h"

#include <glfw/glfw3.h>

namespace byhj
{

Plane::Plane()
{

}

Plane::~Plane()
{

}

void Plane::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Plane::Render()
{
	glUseProgram(program_exposure);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_src);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(0);
}


void Plane::Shutdown()
{

}

void Plane::init_buffer()
{

}

void Plane::init_vertexArray()
{

}

void Plane::init_shader()
{

	exposureShader.init();
	exposureShader.attach(GL_VERTEX_SHADER, "hdr.vert");
	exposureShader.attach(GL_FRAGMENT_SHADER, "hdr_exposure.frag");
	exposureShader.link();
	program_exposure = exposureShader.GetProgram();
	exposure_loc = glGetUniformLocation(program_exposure, "exposure");


}

void Plane::init_texture()
{
	tex_src = sb6::ktx::load("../../../media/textures/treelights_2k.ktx");
	glBindTexture(GL_TEXTURE_2D, tex_src);
}

}