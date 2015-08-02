#include "Cube.h"
#include <glfw/glfw3.h>
#include "ogl/oglDebug.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define DEPTH_TEXTURE_SIZE      4096
#define FRUSTUM_DEPTH           1000


namespace byhj
{

Cube::Cube()
{

}

Cube::~Cube()
{

}

void Cube::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Cube::Render(GLfloat aspect)
{

	static const GLfloat zeros[] ={ 0.0f, 0.0f, 0.0f, 0.0f };

	static double last_time = 0.0;
	static double total_time = 0.0;
	float currentTime = glfwGetTime() / 100.0f;
	if (!paused)
		total_time += (currentTime - last_time);
	last_time = currentTime;

	const float f = (float)total_time + 30.0f;

	glm::vec3 light_position = glm::vec3(20.0f, 20.0f, 20.0f);
	glm::vec3 view_position = glm::vec3(0.0f, 0.0f, 150.0f);

	light_proj_matrix = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 200.0f);
	light_view_matrix = glm::lookAt(light_position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	camera_proj_matrix = glm::perspective(50.0f, aspect, 1.0f, 200.0f);

	camera_view_matrix[0] = glm::lookAt(view_position - glm::vec3(separation, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -50.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	camera_view_matrix[1] = glm::lookAt(view_position + glm::vec3(separation, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -50.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	objects[0].model_matrix = glm::rotate(glm::mat4(1.0f), 14.5f, glm::vec3(0.0f, 1.0f, 0.0f))*
		glm::rotate(glm::mat4(1.0f), 20.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 0.0f));

	objects[1].model_matrix = glm::rotate(glm::mat4(1.0f), f * 3.7f, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::translate(glm::mat4(1.0f), glm::vec3(sinf(f * 0.37f) * 12.0f, cosf(f * 0.37f) * 12.0f, 0.0f))*
		glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));

	objects[2].model_matrix = glm::rotate(glm::mat4(1.0f), f * 6.45f, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::translate(glm::mat4(1.0f), glm::vec3(sinf(f * 0.25f) * 10.0f, cosf(f * 0.25f) * 10.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), f * 99.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));

	objects[3].model_matrix = glm::rotate(glm::mat4(1.0f), f * 5.25f, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::translate(glm::mat4(1.0f), glm::vec3(sinf(f * 0.51f) * 14.0f, cosf(f * 0.51f) * 14.0f, 0.0f))*
		glm::rotate(glm::mat4(1.0f), f * 120.3f, glm::vec3(0.707106f, 0.0f, 0.707106f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));

	glEnable(GL_DEPTH_TEST);

	render_scene(total_time);
}


void Cube::Shutdown()
{
}

void Cube::init_buffer()
{
	static const char * const object_names[] =
	{
		"../../../media/objects/dragon.sbm",
		"../../../media/objects/sphere.sbm",
		"../../../media/objects/cube.sbm",
		"../../../media/objects/torus.sbm"
	};

	for (int i = 0; i < OBJECT_COUNT; i++)
	{
		objects[i].obj.load(object_names[i]);
	}

	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &quad_vao);
	glBindVertexArray(quad_vao);

}

void Cube::init_vertexArray()
{

}

void Cube::init_shader()
{
	StereoShader.init();
	StereoShader.attach(GL_VERTEX_SHADER, "stereo.vert");
	StereoShader.attach(GL_FRAGMENT_SHADER, "stereo.frag");
	StereoShader.link();
	view_program = StereoShader.GetProgram();

	uniforms.view.proj_matrix = glGetUniformLocation(view_program, "proj_matrix");
	uniforms.view.mv_matrix = glGetUniformLocation(view_program, "mv_matrix");
	uniforms.view.shadow_matrix = glGetUniformLocation(view_program, "shadow_matrix");
	uniforms.view.full_shading = glGetUniformLocation(view_program, "full_shading");
	uniforms.view.specular_albedo = glGetUniformLocation(view_program, "specular_albedo");
	uniforms.view.diffuse_albedo = glGetUniformLocation(view_program, "diffuse_albedo");

}

void Cube::render_scene(double currentTime)
{
	static const GLfloat ones[] ={ 1.0f };
	static const GLfloat zero[] ={ 0.0f };
	static const GLfloat gray[] ={ 0.1f, 0.1f, 0.1f, 0.0f };
	static const glm::mat4 scale_bias_matrix = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
		glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	glm::mat4 light_vp_matrix = light_proj_matrix * light_view_matrix;
	glm::mat4 shadow_sbpv_matrix = scale_bias_matrix * light_proj_matrix * light_view_matrix;


	glClearBufferfv(GL_COLOR, 0, gray);
	glUseProgram(view_program);
	glActiveTexture(GL_TEXTURE0);
	glUniformMatrix4fv(uniforms.view.proj_matrix, 1, GL_FALSE, &camera_proj_matrix[0][0]);
	glDrawBuffer(GL_BACK);

	int i, j;

	static const glm::vec3 diffuse_colors[] =
	{
		glm::vec3(1.0f, 0.6f, 0.3f),
		glm::vec3(0.2f, 0.8f, 0.9f),
		glm::vec3(0.3f, 0.9f, 0.4f),
		glm::vec3(0.5f, 0.2f, 1.0f)
	};

	for (j = 0; j < 2; j++)
	{
		static const GLenum buffs[] ={ GL_BACK_LEFT, GL_BACK_RIGHT };
		glDrawBuffer(buffs[j]);

		glClearBufferfv(GL_COLOR, 0, gray);
		glClearBufferfv(GL_DEPTH, 0, ones);
		for (i = 0; i < 4; i++)
		{
			glm::mat4& model_matrix = objects[i].model_matrix;
			glm::mat4 shadow_matrix = shadow_sbpv_matrix * model_matrix;
			glUniformMatrix4fv(uniforms.view.shadow_matrix, 1, GL_FALSE, &shadow_matrix[0][0]);
			glUniformMatrix4fv(uniforms.view.mv_matrix, 1, GL_FALSE, &(camera_view_matrix[j] * objects[i].model_matrix)[0][0]);
			glUniform1i(uniforms.view.full_shading, mode == RENDER_FULL ? 1 : 0);
			glUniform3fv(uniforms.view.diffuse_albedo, 1, &(diffuse_colors[i])[0]);
			objects[i].obj.render();
		}
	}
}

}