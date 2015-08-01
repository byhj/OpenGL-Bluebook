#include "Terrian.h"
#include <glfw/glfw3.h>
#include "ogl/ktx.h"
#include "ogl/vmath.h"

#include <iostream>
#include <vector> 

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace byhj
{

	Terrian::Terrian()
{

}

Terrian::~Terrian()
{

}

void Terrian::Init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
}

void Terrian::Render(GLfloat aspect)
{

	float currentTime =  glfwGetTime();

	float t = (float)currentTime * 0.03f;
	float r = sinf(t * 5.37f) * 15.0f + 16.0f;
	float h = cosf(t * 4.79f) * 2.0f + 5.2f;

	vmath::mat4 mv_matrix = /* vmath::translate(0.0f, 0.0f, -1.4f) *
							vmath::translate(0.0f, -0.4f, 0.0f) * */
							// vmath::rotate((float)currentTime * 6.0f, 0.0f, 1.0f, 0.0f) *
	vmath::lookat(vmath::vec3(sinf(t) * r, h, cosf(t) * r), vmath::vec3(0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));
	vmath::mat4 proj_matrix = vmath::perspective(60.0f, aspect, 0.1f, 1000.0f);

	glUseProgram(program);

	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, mv_matrix);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(uniforms.mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix);

	/*
	glm::mat4 mv_matrix  = glm::lookAt( glm::vec3(sinf(t) * r, h, cosf(t) * r), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(60.0f, GetAspect(), 0.1f, 1000.0f);

	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, glm::value_ptr(proj_matrix));
	glUniformMatrix4fv(uniforms.mvp_matrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
	*/
	glUniform1f(uniforms.dmap_depth, enable_displacement ? dmap_depth : 0.0f);
	glUniform1i(uniforms.enable_fog, enable_fog ? 1 : 0);
	if (wireframe)
	   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);


	glUseProgram(0);

}

void Terrian::Shutdown()
{
	glDeleteProgram(program);
}


void Terrian::init_buffer()
{

}

void Terrian::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}


void Terrian::init_shader()
{
	dispmapShader.init();
	dispmapShader.attach(GL_VERTEX_SHADER, "dispmap.vert");
	dispmapShader.attach(GL_TESS_CONTROL_SHADER, "dispmap.tcs");
	dispmapShader.attach(GL_TESS_EVALUATION_SHADER, "dispmap.tes");
	dispmapShader.attach(GL_FRAGMENT_SHADER, "dispmap.frag");
	dispmapShader.link();
	program = dispmapShader.GetProgram();

	uniforms.mv_matrix = glGetUniformLocation(program, "mv_matrix");
	uniforms.mvp_matrix = glGetUniformLocation(program, "mvp_matrix");
	uniforms.proj_matrix = glGetUniformLocation(program, "proj_matrix");
	uniforms.dmap_depth = glGetUniformLocation(program, "dmap_depth");
	uniforms.enable_fog = glGetUniformLocation(program, "enable_fog");
	dmap_depth = 6.0f;


	glActiveTexture(GL_TEXTURE0);
	tex_displacement = sb6::ktx::load("../../../media/textures/terragen1.ktx");
	glActiveTexture(GL_TEXTURE1);
	tex_color = sb6::ktx::load("../../../media/textures/terragen_color.ktx");

	uniforms.mv_matrix = glGetUniformLocation(program, "mv_matrix");
	uniforms.proj_matrix = glGetUniformLocation(program, "proj_matrix");
}

}