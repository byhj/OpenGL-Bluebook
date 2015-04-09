#include <gl/glew.h>
#include <gl/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>
#include <sb6.h>
#include <object.cpp>

class FragmentList: public sb6::Application
{
public:
	FragmentList(){};
	~FragmentList(){}
public:
	virtual void init();
	virtual void render();
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	struct Transform
	{
		glm::mat4 mv_matrix;
		glm::mat4 view_matrix;
		glm::mat4 proj_matrix;
	};
private:
	GLuint clear_prog, append_prog, resolve_prog;
	GLuint mvp_loc;
	Shader ClearShader, AppendShader, ResolveShader;
	Object object;
	GLuint vao, ubo, texture, fragment_buffer, atomic_counter_buffer;
};

void FragmentList::init_buffer()
{
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Transform), NULL, GL_DYNAMIC_DRAW);

	object.load("../media/objects/dragon.sbm");

	glGenBuffers(1, &fragment_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fragment_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 1024 * 1024 * 16, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &atomic_counter_buffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_buffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, 4, NULL, GL_DYNAMIC_COPY);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, 1024, 1024);

}
void FragmentList::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

}
void FragmentList::init()
{
	init_shader();
	init_vertexArray();
	init_buffer();
}

void FragmentList::render()
{
	static const GLfloat zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	static const GLfloat ones[] = { 1.0f };
	const float f = (float)glfwGetTime();
	glViewport(0, 0, 1300, 960);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(clear_prog);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(append_prog);
	glBindVertexArray(vao);
	glm::mat4 model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, 7.0f, 7.0f));
	glm::vec3 view_position = glm::vec3(cosf(f * 0.35f) * 120.0f, cosf(f * 0.4f) * 30.0f, sinf(f * 0.35f) * 120.0f);
	glm::mat4 view_matrix = glm::lookAt(view_position, glm::vec3(0.0f, 30.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mv_matrix = view_matrix * model_matrix;
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, &(proj_matrix * mv_matrix)[0][0]);

	static const unsigned int zero = 0;
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter_buffer);
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, fragment_buffer);

	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
	object.render();
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(resolve_prog);
	glBindVertexArray(vao);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void FragmentList::init_shader()
{
	ClearShader.attach(GL_VERTEX_SHADER, "clear.vert");
	ClearShader.attach(GL_FRAGMENT_SHADER, "clear.frag");
	ClearShader.link();
	clear_prog = ClearShader.program;

	AppendShader.attach(GL_VERTEX_SHADER, "append.vert");
	AppendShader.attach(GL_FRAGMENT_SHADER, "append.frag");
	AppendShader.link();
	append_prog = AppendShader.program;
	mvp_loc = glGetUniformLocation(append_prog, "mvp_matrix");

	ResolveShader.attach(GL_VERTEX_SHADER, "resolve.vert");
	ResolveShader.attach(GL_FRAGMENT_SHADER, "resolve.frag");
	ResolveShader.link();
	resolve_prog = ResolveShader.program;
}

DECLARE_MAIN(FragmentList);