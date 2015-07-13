#include <GL/glew.h>
#include "ogl/oglApp.h"
#include <ogl/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


class FBO : public byhj::Application
{
public:
	void init_shader();
	void v_Init();
	void v_Render();

private:
    GLuint   fbo_prog;
    GLuint   base_prog;

    GLuint   position_buffer;
    GLuint   index_buffer;
    GLuint   fbo, vao;
    GLuint   color_texture;
    GLuint   depth_texture;

    GLint    fbo_mv_loc, fbo_proj_loc;
	GLint    base_mv_loc, base_proj_loc;
    Shader FboShader;
    Shader BaseShader;
};

CALL_MAIN(FBO);

static const GLushort vertex_indices[] = 
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

static const GLfloat vertex_data[] = 
{
	// Position                 Tex Coord
	-0.25f, -0.25f, 0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
	0.25f, -0.25f, -0.25f, 1.0f, 0.0f,

	0.25f, -0.25f, -0.25f, 1.0f, 0.0f,
	0.25f, -0.25f, 0.25f, 1.0f, 1.0f,
	-0.25f, -0.25f, 0.25f, 0.0f, 1.0f,

	0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
	0.25f, 0.25f, -0.25f, 1.0f, 0.0f,
	0.25f, -0.25f, 0.25f, 0.0f, 1.0f,

	0.25f, 0.25f, -0.25f, 1.0f, 0.0f,
	0.25f, 0.25f, 0.25f, 1.0f, 1.0f,
	0.25f, -0.25f, 0.25f, 0.0f, 1.0f,

	0.25f, 0.25f, -0.25f, 1.0f, 0.0f,
	-0.25f, 0.25f, -0.25f, 0.0f, 0.0f,
	0.25f, 0.25f, 0.25f, 1.0f, 1.0f,

	-0.25f, 0.25f, -0.25f, 0.0f, 0.0f,
	-0.25f, 0.25f, 0.25f, 0.0f, 1.0f,
	0.25f, 0.25f, 0.25f, 1.0f, 1.0f,

	-0.25f, 0.25f, -0.25f, 1.0f, 0.0f,
	-0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
	-0.25f, 0.25f, 0.25f, 1.0f, 1.0f,

	-0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
	-0.25f, -0.25f, 0.25f, 0.0f, 1.0f,
	-0.25f, 0.25f, 0.25f, 1.0f, 1.0f,

	-0.25f, 0.25f, -0.25f, 0.0f, 1.0f,
	0.25f, 0.25f, -0.25f, 1.0f, 1.0f,
	0.25f, -0.25f, -0.25f, 1.0f, 0.0f,

	0.25f, -0.25f, -0.25f, 1.0f, 0.0f,
	-0.25f, -0.25f, -0.25f, 0.0f, 0.0f,
	-0.25f, 0.25f, -0.25f, 0.0f, 1.0f,

	-0.25f, -0.25f, 0.25f, 0.0f, 0.0f,
	0.25f, -0.25f, 0.25f, 1.0f, 0.0f,
	0.25f, 0.25f, 0.25f, 1.0f, 1.0f,

	0.25f, 0.25f, 0.25f, 1.0f, 1.0f,
	-0.25f, 0.25f, 0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.25f, 0.0f, 0.0f,
};

void FBO::init_shader()
{
	//fbo
	FboShader.init();
	FboShader.attach(GL_VERTEX_SHADER, "cube.vert");
	FboShader.attach(GL_FRAGMENT_SHADER, "cube0.frag");
	FboShader.link();
	fbo_prog= FboShader.GetProgram();
	fbo_mv_loc   = glGetUniformLocation(fbo_prog, "mv_matrix");
	fbo_proj_loc = glGetUniformLocation(fbo_prog, "proj_matrix");

	
	BaseShader.attach(GL_VERTEX_SHADER, "cube.vert");
	BaseShader.attach(GL_FRAGMENT_SHADER, "cube1.frag");
	BaseShader.link();
	base_prog = BaseShader.GetProgram();
	base_mv_loc = glGetUniformLocation(base_prog, "mv_matrix");
	base_proj_loc = glGetUniformLocation(base_prog, "proj_matrix");

}

void FBO::v_Init(void)
{
	init_shader();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &position_buffer);  //向shader传入数据
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(vertex_indices), vertex_indices,GL_STATIC_DRAW);

	glEnable(GL_CULL_FACE); 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); 

	//Create the framebuffer, render to texture
	glGenFramebuffers(1, &fbo); 
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//Create color buffer  to save the fbo data
	glGenTextures(1, &color_texture);  
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  
   //Create depth buffer for fbo
	glGenTextures(1, &depth_texture);   
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexStorage2D(GL_TEXTURE_2D, 9, GL_DEPTH_COMPONENT32F, 512, 512);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

	static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers); 
}

void FBO::v_Render(void)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	static const GLfloat green[] = { 0.0f, 0.1f, 0.0f, 1.0f };
	static const GLfloat blue[] = { 0.0f, 0.0f, 0.3f, 1.0f };
	static const GLfloat one = 1.0f;

	static float currentTime;
	currentTime  = glfwGetTime() / 100.0f;
	float time = (float)currentTime * 0.3f;

	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -2.0f)) 
		                * glm::translate(glm::mat4(1.0), glm::vec3(sinf(2.1f * time)*0.5f, cosf(1.7f*time)*0.5f,
		                                 sinf(1.3f * time) *cosf(1.5f * time) * 2.0f));

	mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
	
	//We render fbo first, load the framebuffer file as a texture 
	FboShader.use(); 
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); //选择绘制的fbo
	glViewport(0, 0, 512, 512);
	glClearBufferfv(GL_COLOR, 0, green);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glUniformMatrix4fv(fbo_mv_loc, 1, 0, glm::value_ptr(mv_matrix));
	glUniformMatrix4fv(fbo_proj_loc, 1, 0, glm::value_ptr(proj_matrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);  //绘制立方体

	//use fbo texture to render the base scene
	BaseShader.use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	glViewport(0, 0, GetScreenWidth(), GetScreenHeight()); 
	glClearBufferfv(GL_COLOR, 0, blue);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glBindTexture(GL_TEXTURE_2D, color_texture); 
	
	glUniformMatrix4fv(base_proj_loc, 1, 0, glm::value_ptr(proj_matrix));
	glUniformMatrix4fv(base_mv_loc, 1, 0, glm::value_ptr(mv_matrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);  

}


