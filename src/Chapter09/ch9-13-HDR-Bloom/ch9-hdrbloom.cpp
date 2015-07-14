#include <GL/glew.h>
#include "ogl/oglApp.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ogl/vmath.h"
#include "ogl/shader.h"
#include "ogl/ktx.cpp"
#include "ogl/object.cpp"

class Bloom: public byhj::Application
{
public:
	void init_shader();
	void init_texture();
	void init_buffer();

	void v_Init();
	void v_Render();
	void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode);
private:

	enum
	{
		MAX_SCENE_WIDTH     = 2048,
		MAX_SCENE_HEIGHT    = 2048,
		SPHERE_COUNT        = 32,
	};

	GLuint      tex_src;
	GLuint      tex_lut;

	GLuint      render_fbo;
	GLuint      filter_fbo[2];

	GLuint      tex_scene;
	GLuint      tex_brightpass;
	GLuint      tex_depth;
	GLuint      tex_filter[2];

	GLuint      program_render;
	GLuint      program_filter;
	GLuint      program_resolve;
	GLuint      vao;
	float       exposure;
	int         mode;
	bool        paused;
	float       bloom_factor;
	bool        show_bloom;
	bool        show_scene;
	bool        show_prefilter;
	float       bloom_thresh_min;
	float       bloom_thresh_max;

	struct
	{
		struct
		{
			int bloom_thresh_min;
			int bloom_thresh_max;
		} scene;
		struct
		{
			int exposure;
			int bloom_factor;
			int scene_factor;
		} resolve;
	} uniforms;

	GLuint      ubo_transform;
	GLuint      ubo_material;

	Shader ResloveShader;
	Shader FilterShader;
	Shader SceneShader;
	sb6::Object   obj;
	GLuint explode_factor_location;
	bool depthClamp;
};

CALL_MAIN(Bloom);

void Bloom::init_shader(void)
{

	SceneShader.attach( GL_VERTEX_SHADER,  "scene.vert");
	SceneShader.attach( GL_FRAGMENT_SHADER, "scene.frag");
	SceneShader.link();
	program_render = SceneShader.GetProgram();
	uniforms.scene.bloom_thresh_min = glGetUniformLocation(program_render, "bloom_thresh_min");
	uniforms.scene.bloom_thresh_max = glGetUniformLocation(program_render, "bloom_thresh_max");

	FilterShader.attach( GL_VERTEX_SHADER, "filter.vert");
	FilterShader.attach( GL_FRAGMENT_SHADER, "filter.frag");
	FilterShader.link();
	program_filter = FilterShader.GetProgram();


	ResloveShader.attach(GL_VERTEX_SHADER, "resolve.vert");
	ResloveShader.attach(GL_FRAGMENT_SHADER, "resolve.frag");
	ResloveShader.link();
	program_resolve = ResloveShader.GetProgram();
	uniforms.resolve.exposure = glGetUniformLocation(program_resolve, "exposure");
	uniforms.resolve.bloom_factor = glGetUniformLocation(program_resolve, "bloom_factor");
	uniforms.resolve.scene_factor = glGetUniformLocation(program_resolve, "scene_factor");
}


void Bloom::v_Init()
{
	init_shader();

	int i;
	static const GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	static const GLfloat exposureLUT[20]   = { 11.0f, 6.0f, 3.2f, 2.8f, 2.2f, 1.90f, 1.80f, 1.80f, 1.70f, 1.70f,  1.60f, 1.60f, 1.50f, 1.50f, 1.40f, 1.40f, 1.30f, 1.20f, 1.10f, 1.00f };

	glGenFramebuffers(1, &render_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, render_fbo);

	glGenTextures(1, &tex_scene);
	glBindTexture(GL_TEXTURE_2D, tex_scene);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_scene, 0);
	glGenTextures(1, &tex_brightpass);
	glBindTexture(GL_TEXTURE_2D, tex_brightpass);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, tex_brightpass, 0);
	glGenTextures(1, &tex_depth);
	glBindTexture(GL_TEXTURE_2D, tex_depth);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex_depth, 0);
	glDrawBuffers(2, buffers);

	glGenFramebuffers(2, &filter_fbo[0]);
	glGenTextures(2, &tex_filter[0]);
	for (i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, filter_fbo[i]);
		glBindTexture(GL_TEXTURE_2D, tex_filter[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, i ? MAX_SCENE_WIDTH : MAX_SCENE_HEIGHT, i ? MAX_SCENE_HEIGHT : MAX_SCENE_WIDTH);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_filter[i], 0);
		glDrawBuffers(1, buffers);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &tex_lut);
	glBindTexture(GL_TEXTURE_1D, tex_lut);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, 20);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 20, GL_RED, GL_FLOAT, exposureLUT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	obj.load("../../../media/objects/sphere.sbm");

	glGenBuffers(1, &ubo_transform);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_transform);
	glBufferData(GL_UNIFORM_BUFFER, (2 + SPHERE_COUNT) * sizeof(vmath::mat4), NULL, GL_DYNAMIC_DRAW);

	struct material
	{
		vmath::vec3     diffuse_color;
		unsigned int    : 32;           // pad
		vmath::vec3     specular_color;
		float           specular_power;
		vmath::vec3     ambient_color;
		unsigned int    : 32;           // pad
	};

	glGenBuffers(1, &ubo_material);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_material);
	glBufferData(GL_UNIFORM_BUFFER, SPHERE_COUNT * sizeof(material), NULL, GL_STATIC_DRAW);

	material * m = (material *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, SPHERE_COUNT * sizeof(material), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	float ambient = 0.002f;
	for (i = 0; i < SPHERE_COUNT; i++)
	{
		float fi = 3.14159267f * (float)i / 8.0f;
		m[i].diffuse_color  = vmath::vec3(sinf(fi) * 0.5f + 0.5f, sinf(fi + 1.345f) * 0.5f + 0.5f, sinf(fi + 2.567f) * 0.5f + 0.5f);
		m[i].specular_color = vmath::vec3(2.8f, 2.8f, 2.9f);
		m[i].specular_power = 30.0f;
		m[i].ambient_color  = vmath::vec3(ambient * 0.025f);
		ambient *= 1.5f;
	}
	glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void Bloom::v_Render()
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;
	int i;
	static double last_time = 0.0;
	static double total_time = 0.0;
	float currentTime = glfwGetTime();
	if (!paused)
		total_time += (currentTime - last_time);
	last_time = currentTime;
	float t = (float)total_time;

	glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

	glBindFramebuffer(GL_FRAMEBUFFER, render_fbo);
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_COLOR, 1, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glUseProgram(program_render);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_transform);
	struct transforms_t
	{
		vmath::mat4 mat_proj;
		vmath::mat4 mat_view;
		vmath::mat4 mat_model[SPHERE_COUNT];
	} * transforms = (transforms_t *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(transforms_t), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	transforms->mat_proj = vmath::perspective(50.0f, GetAspect(), 1.0f, 1000.0f);
	transforms->mat_view = vmath::translate(0.0f, 0.0f, -20.0f);
	for (i = 0; i < SPHERE_COUNT; i++)
	{
		float fi = 3.141592f * (float)i / 16.0f;
		// float r = cosf(fi * 0.25f) * 0.4f + 1.0f;
		float r = (i & 2) ? 0.6f : 1.5f;
		transforms->mat_model[i] = vmath::translate(cosf(t + fi) * 5.0f * r, sinf(t + fi * 4.0f) * 4.0f, sinf(t + fi) * 5.0f * r);
	}
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_material);

	glUniform1f(uniforms.scene.bloom_thresh_min, bloom_thresh_min);
	glUniform1f(uniforms.scene.bloom_thresh_max, bloom_thresh_max);

	obj.render(SPHERE_COUNT);

	glDisable(GL_DEPTH_TEST);

	glUseProgram(program_filter);

	glBindVertexArray(vao);

	glBindFramebuffer(GL_FRAMEBUFFER, filter_fbo[0]);
	glBindTexture(GL_TEXTURE_2D, tex_brightpass);
	glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindFramebuffer(GL_FRAMEBUFFER, filter_fbo[1]);
	glBindTexture(GL_TEXTURE_2D, tex_filter[0]);
	glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(program_resolve);

	glUniform1f(uniforms.resolve.exposure, exposure);
	if (show_prefilter)
	{
		glUniform1f(uniforms.resolve.bloom_factor, 0.0f);
		glUniform1f(uniforms.resolve.scene_factor, 1.0f);
	}
	else
	{
		glUniform1f(uniforms.resolve.bloom_factor, show_bloom ? bloom_factor : 0.0f);
		glUniform1f(uniforms.resolve.scene_factor, show_scene ? 1.0f : 0.0f);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_filter[1]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, show_prefilter ? tex_brightpass : tex_scene);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Bloom::v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (!action)
		return;

	switch (key)
	{
	case '1':
	case '2':
	case '3':
		mode = key - '1';
		break;
	case 'B':
		show_bloom = !show_bloom;
		break;
	case 'V':
		show_scene = !show_scene;
		break;
	case 'A':
		bloom_factor += 0.1f;
		break;
	case 'Z':
		bloom_factor -= 0.1f;
		break;
	case 'S':
		bloom_thresh_min += 0.1f;
		break;
	case 'X':
		bloom_thresh_min -= 0.1f;
		break;
	case 'D':
		bloom_thresh_max += 0.1f;
		break;
	case 'C':
		bloom_thresh_max -= 0.1f;
		break;
	case 'N':
		show_prefilter = !show_prefilter;
		break;
	case 'M':
		mode = (mode + 1) % 3;
		break;
	case 'P':
		paused = !paused;
		break;
	case GLFW_KEY_KP_ADD:
		exposure *= 1.1f;
		break;
	case GLFW_KEY_KP_SUBTRACT:
		exposure /= 1.1f;
		break;
	}
}