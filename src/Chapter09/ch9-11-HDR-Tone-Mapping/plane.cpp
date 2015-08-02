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
	glUseProgram(program_adaptive);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, tex_lut);
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
	naiveShader.init();
	naiveShader.attach(GL_VERTEX_SHADER, "hdr.vert");
	naiveShader.attach(GL_FRAGMENT_SHADER, "hdr_naive.frag");
	naiveShader.link();
	program_naive = naiveShader.GetProgram();

	exposureShader.init();
	exposureShader.attach(GL_VERTEX_SHADER, "hdr.vert");
	exposureShader.attach(GL_FRAGMENT_SHADER, "hdr_exposure.frag");
	exposureShader.link();
	program_exposure = exposureShader.GetProgram();
	exposure_loc = glGetUniformLocation(program_exposure, "exposure");

	adaptiveShader.init();
	adaptiveShader.attach(GL_VERTEX_SHADER, "hdr.vert");
	adaptiveShader.attach(GL_FRAGMENT_SHADER, "hdr_adaptive.frag");
	adaptiveShader.link();
	program_adaptive = adaptiveShader.GetProgram();
}

void Plane::init_texture()
{
	tex_src = sb6::ktx::load("../../../media/textures/treelights_2k.ktx");
	glBindTexture(GL_TEXTURE_2D, tex_src);
	static const GLfloat exposureLUT[20]   ={ 11.0f, 6.0f, 3.2f, 2.8f, 2.2f, 1.90f, 1.80f, 1.80f, 1.70f, 1.70f,  1.60f, 1.60f, 1.50f, 1.50f, 1.40f, 1.40f, 1.30f, 1.20f, 1.10f, 1.00f };

	glGenTextures(1, &tex_lut);
	glBindTexture(GL_TEXTURE_1D, tex_lut);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, 20);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 20, GL_RED, GL_FLOAT, exposureLUT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
}

}