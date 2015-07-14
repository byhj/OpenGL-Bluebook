#include <GL/glew.h>

#include "ogl/oglApp.h"
#include "ogl/shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Blend: public byhj::Application
{
public:
	void init_shader();
	void v_Init();
	void v_Render();
	void init_buffer();
	void init_vertexArray();
private:
  Shader cubeShader;
  GLuint program, mvp_loc, vao, vbo, ibo, ubo;
};

CALL_MAIN(Blend);

static const GLushort ElementData[] =
{
            0, 1, 2,
            2, 1, 3,
            2, 3, 4,
            4, 3, 5,
            4, 5, 6,
            6, 5, 7,
            6, 7, 0,
            0, 7, 1,
            6, 0, 2,
            2, 4, 6,
            7, 5, 3,
            7, 3, 1
};

static const GLfloat VertexData[] =
{
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
             0.25f, -0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
};

void Blend::init_shader()
{
	cubeShader.init();
	cubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	cubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	cubeShader.link();
	cubeShader.use();
	program = cubeShader.GetProgram();
	mvp_loc = glGetUniformLocation(program, "mvp_matrix");
}

void Blend::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ARRAY_BUFFER, ibo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);

	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

}

void Blend::init_vertexArray() 
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glEnableVertexAttribArray(0);
}

void Blend::v_Init()
{  
   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   init_shader();
   init_buffer();
   init_vertexArray();
}


void Blend::v_Render()
{
	static const GLfloat orange[] = { 0.6f, 0.4f, 0.1f, 1.0f };
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_COLOR, 0, orange);
	glClearBufferfv(GL_DEPTH, 0, &one);

	static const GLenum blend_func[] =  //19»ìºÏÄ£Ê½
	{
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA,
		GL_ONE_MINUS_CONSTANT_ALPHA,
		GL_SRC_ALPHA_SATURATE,
		GL_SRC1_COLOR,
		GL_ONE_MINUS_SRC1_COLOR,
		GL_SRC1_ALPHA,
		GL_ONE_MINUS_SRC1_ALPHA
	};

	static const int num_blend_funcs = sizeof(blend_func) / sizeof(blend_func[0]);
	static const float x_scale = 20.0f / float(num_blend_funcs);
	static const float y_scale = 16.0f / float(num_blend_funcs);

	glUseProgram(program);
	glBindVertexArray(vao);
	glEnable(GL_BLEND);
	glBlendColor(0.2f, 0.5f, 0.7f, 0.5f);  

	glm::mat4 proj_matrix = glm::perspective(50.0f, GetAspect(), 0.1f, 1000.0f);

	float currentTime = glfwGetTime() / 100.0f;
    for (int j = 0; j < num_blend_funcs; j++)  
	{   for (int i = 0; i < num_blend_funcs; i++) 
	   {
		  float time = currentTime;
		  glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(9.5f - x_scale * float(i),
			                                   7.5f - y_scale * float(j), -70.0f));
		   //We set the cube position with offset
		   mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * -45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		   mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * -21.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		   glUniformMatrix4fv(mvp_loc, 1, 0, glm::value_ptr(proj_matrix * mv_matrix));
		 
		   //Call the different blend function
		   glBlendFunc(blend_func[i], blend_func[j]);  
		   glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	    }
	}
}


