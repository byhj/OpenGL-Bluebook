#include "Texture.h"

#include <glfw/glfw3.h>

namespace byhj
{

Texture::Texture()
{

}

Texture::~Texture()
{

}

void Texture::Init(int sw, int sh)
{
	init_buffer(sw, sh);
	init_vertexArray();
	init_shader();
	init_texture();
}

void Texture::Render(float aspect)
{
	static const GLfloat zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	static const GLfloat ones[] = { 1.0f };
	const float f = (float)glfwGetTime();

	////////////////////////////Clear/////////////////////////////////
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(clear_program);
	glBindVertexArray(dummy_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(0);

	////////////////////////////Append/////////////////////////////////
	glUseProgram(append_program);

	vmath::mat4 model_matrix = vmath::scale(7.0f);
	vmath::vec3 view_position = vmath::vec3(cosf(f * 0.35f) * 120.0f, cosf(f * 0.4f) * 30.0f, sinf(f * 0.35f) * 120.0f);
	vmath::mat4 view_matrix = vmath::lookat(view_position, vmath::vec3(0.0f, 30.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));

	vmath::mat4 mv_matrix = view_matrix * model_matrix;
	vmath::mat4 proj_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);

	glUniformMatrix4fv(uniforms.mvp, 1, GL_FALSE, proj_matrix * mv_matrix);

	static const unsigned int zero = 0;
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter_buffer);
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, fragment_buffer);

	glBindImageTexture(0, head_pointer_image, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	object.render();

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(0);

	//////////////////////////////////Resolve/////////////////////////////////
	glUseProgram(resolve_program);
	glBindVertexArray(dummy_vao);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(0);
}

void Texture::Shutdown()
{
	glDeleteProgram(append_program);
	glDeleteProgram(resolve_program);
	glDeleteProgram(clear_program);
}

void Texture::update()
{

}


void Texture::init_buffer(int sw, int sh)
{
	glGenBuffers(1, &uniforms_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniforms_buffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_block), NULL, GL_DYNAMIC_DRAW);

	object.load("../../../media/objects/dragon.sbm");

	glGenBuffers(1, &fragment_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fragment_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sw * sh * 16, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &atomic_counter_buffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_buffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, 4, NULL, GL_DYNAMIC_COPY);

	glGenTextures(1, &head_pointer_image);
	glBindTexture(GL_TEXTURE_2D, head_pointer_image);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, sw, sh);

	glGenVertexArrays(1, &dummy_vao);
	glBindVertexArray(dummy_vao);
}

void Texture::init_vertexArray()
{

}

void Texture::init_shader()
{
	ClearShader.init();
	ClearShader.attach(GL_VERTEX_SHADER, "clear.vert");
	ClearShader.attach(GL_FRAGMENT_SHADER, "clear.frag");
	ClearShader.link();
	clear_program = ClearShader.GetProgram();
	
	AppendShader.init();
	AppendShader.attach(GL_VERTEX_SHADER, "append.vert");
	AppendShader.attach(GL_FRAGMENT_SHADER, "append.frag");
	AppendShader.link();
	append_program = AppendShader.GetProgram();
	uniforms.mvp = glGetUniformLocation(append_program, "mvp");

	ResolveShader.init();
	ResolveShader.attach(GL_VERTEX_SHADER, "resolve.vert");
	ResolveShader.attach(GL_FRAGMENT_SHADER, "resolve.frag");
	ResolveShader.link();
	resolve_program = ResolveShader.GetProgram();
}


void Texture::init_texture()
{

}

}