#include "Sphere.h"
#include <glfw/glfw3.h>

#include "ogl/ktx.h"
#include "ogl/oglDebug.h"
enum
{
	MAX_DISPLAY_WIDTH       = 2048,
	MAX_DISPLAY_HEIGHT      = 2048,
	NUM_LIGHTS              = 64,
	NUM_INSTANCES           = (15 * 15)
};


namespace byhj
{

Sphere::Sphere()
{

}

Sphere::~Sphere()
{

}

void Sphere::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Sphere::Render(GLfloat aspect)
{

	static const GLuint uint_zeros[] ={ 0, 0, 0, 0 };
	static const GLfloat float_zeros[] ={ 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat float_ones[] ={ 1.0f, 1.0f, 1.0f, 1.0f };
	static const GLenum draw_buffers[] ={ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	int i, j;
	static double last_time = 0.0;
	static double total_time = 0.0;

	float currentTime = glfwGetTime();

	if (!paused)
	{
		total_time += (currentTime - last_time);
	}
	else
	{
#ifdef _WIN32
		Sleep(10);
#endif
	}
	last_time = currentTime;

	float t = (float)total_time;

	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);
	glDrawBuffers(2, draw_buffers);
	glClearBufferuiv(GL_COLOR, 0, uint_zeros);
	glClearBufferuiv(GL_COLOR, 1, uint_zeros);
	glClearBufferfv(GL_DEPTH, 0, float_ones);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, render_transform_ubo);
	vmath::mat4 * matrices = reinterpret_cast<vmath::mat4 *>(glMapBufferRange(GL_UNIFORM_BUFFER,
		0,
		(2 + NUM_INSTANCES) * sizeof(vmath::mat4),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

	matrices[0] = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);

	float d = (sinf(t * 0.131f) + 2.0f) * 0.15f;
	vmath::vec3 eye_pos = vmath::vec3(d * 120.0f * sinf(t * 0.11f),
		5.5f,
		d * 120.0f * cosf(t * 0.01f));
	matrices[1] = vmath::lookat(eye_pos,
		vmath::vec3(0.0f, -20.0f, 0.0f),
		vmath::vec3(0.0f, 1.0f, 0.0f));

	for (j = 0; j < 15; j++)
	{
		float j_f = (float)j;
		for (i = 0; i < 15; i++)
		{
			float i_f = (float)i;

			matrices[j * 15 + i + 2] = vmath::translate((i - 7.5f) * 7.0f, 0.0f, (j - 7.5f) * 11.0f);
		}
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glUseProgram(use_nm ? render_program_nm : render_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_nm);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	object.render(NUM_INSTANCES);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[1]);

	if (vis_mode == VIS_OFF)
	{
		glUseProgram(light_program);
	}
	else
	{
		glUseProgram(vis_program);
		glUniform1i(loc_vis_mode, vis_mode);
	}

	glDisable(GL_DEPTH_TEST);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, light_ubo);
	light_t * lights = reinterpret_cast<light_t *>(glMapBufferRange(GL_UNIFORM_BUFFER,
		0,
		NUM_LIGHTS * sizeof(light_t),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
	for (i = 0; i < NUM_LIGHTS; i++)
	{
		float i_f = ((float)i - 7.5f) * 0.1f + 0.3f;
		// t = 0.0f;
		lights[i].position = vmath::vec3(100.0f * sinf(t * 1.1f + (5.0f * i_f)) * cosf(t * 2.3f + (9.0f * i_f)),
			15.0f,
			100.0f * sinf(t * 1.5f + (6.0f * i_f)) * cosf(t * 1.9f + (11.0f * i_f))); // 300.0f * sinf(t * i_f * 0.7f) * cosf(t * i_f * 0.9f) - 600.0f);
		lights[i].color = vmath::vec3(cosf(i_f * 14.0f) * 0.5f + 0.8f,
			sinf(i_f * 17.0f) * 0.5f + 0.8f,
			sinf(i_f * 13.0f) * cosf(i_f * 19.0f) * 0.5f + 0.8f);
		// lights[i].color = vmath::vec3(1.0);
		// vmath::vec3(0.5f, 0.4f, 0.75f);
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glBindVertexArray(fs_quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Sphere::Shutdown()
{
	glDeleteTextures(3, &gbuffer_tex[0]);
	glDeleteFramebuffers(1, &gbuffer);
	glDeleteProgram(render_program);
	glDeleteProgram(light_program);
}


void Sphere::init_buffer()
{
	object.load("../../../media/objects/ladybug.sbm");


}

void Sphere::init_vertexArray()
{

}

void Sphere::init_shader()
{
	RenderShader.init();
	RenderShader.attach(GL_VERTEX_SHADER, "render.vert");
	RenderShader.attach(GL_FRAGMENT_SHADER, "render.frag");
	RenderShader.link();
	render_program = RenderShader.GetProgram();

	RenderNMShader.init();
	RenderNMShader.attach(GL_VERTEX_SHADER, "render-nm.vert");
	RenderNMShader.attach(GL_FRAGMENT_SHADER, "render-nm.frag");
	RenderNMShader.link();
	render_program_nm = RenderNMShader.GetProgram();

	LightShader.init();
	LightShader.attach(GL_VERTEX_SHADER, "light.vert");
	LightShader.attach(GL_FRAGMENT_SHADER, "light.frag");
	LightShader.link();
	light_program = LightShader.GetProgram();

	VisShader.init();
	VisShader.attach(GL_FRAGMENT_SHADER, "render-vis.frag");
	VisShader.link();
	vis_program = VisShader.GetProgram();
	loc_vis_mode = glGetUniformLocation(vis_program, "vis_mode");

}

void Sphere::init_texture() 
{
	glGenFramebuffers(1, &gbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);

	glGenTextures(3, gbuffer_tex);
	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32UI, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[1]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, gbuffer_tex[2]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, gbuffer_tex[0], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, gbuffer_tex[1], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, gbuffer_tex[2], 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenVertexArrays(1, &fs_quad_vao);
	glBindVertexArray(fs_quad_vao);

	object.load("../../../media/objects/ladybug.sbm");
	tex_nm = sb6::ktx::load("../../../media/textures/ladybug_nm.ktx");
	tex_diffuse = sb6::ktx::load("../../../media/textures/ladybug_co.ktx");


	glGenBuffers(1, &light_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, light_ubo);
	glBufferData(GL_UNIFORM_BUFFER, NUM_LIGHTS * sizeof(light_t), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &render_transform_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, render_transform_ubo);
	glBufferData(GL_UNIFORM_BUFFER, (2 + NUM_INSTANCES) * sizeof(vmath::mat4), NULL, GL_DYNAMIC_DRAW);
}


}