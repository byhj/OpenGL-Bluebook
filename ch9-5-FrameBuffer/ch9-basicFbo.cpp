#include <GL/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>
static const GLushort vertex_indices[] = //顶点索引
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

static const GLfloat vertex_data[] = //顶点数据和纹理坐标
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

class FBO : public sb6::Application
{
public:
	void init_shader();
	void init();
	void render();
private:
    GLuint          program0;
    GLuint          program1;
    GLuint          position_buffer;
    GLuint          index_buffer;
    GLuint          fbo, vao;
    GLuint          color_texture;
    GLuint          depth_texture;
    GLint           mvLoc0;
    GLint           projLoc0;
    GLuint          mvLoc1;
	GLuint          projLoc1;
    Shader cube0Shader;
    Shader cube1Shader;
};

DECLARE_MAIN(FBO);

void FBO::init_shader()
{
	//作为纹理的fbo对象
	cube0Shader.init();
	cube0Shader.attach(GL_VERTEX_SHADER, "cube.vert");
	cube0Shader.attach(GL_FRAGMENT_SHADER, "cube0.frag");
	cube0Shader.link();
	program0 = cube0Shader.program;
	mvLoc0 = glGetUniformLocation(program0, "mv_matrix");
	projLoc0 = glGetUniformLocation(program0, "proj_matrix");

	//真正绘制
	cube1Shader.attach(GL_VERTEX_SHADER, "cube.vert");
	cube1Shader.attach(GL_FRAGMENT_SHADER, "cube1.frag");
	cube1Shader.link();
	program1 = cube1Shader.program;
	mvLoc1 = glGetUniformLocation(program1, "mv_matrix");
	projLoc1 = glGetUniformLocation(program1, "proj_matrix");

}

void FBO::init(void)
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

	glEnable(GL_CULL_FACE); //背面剔除
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); //<=单前值得通过深度测试

	glGenFramebuffers(1, &fbo); //创建fbo，绑定纹理，用于后面的绘制
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &color_texture);  //创建纹理对象存储颜色缓存
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //选择滤波方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	                                    
	glGenTextures(1, &depth_texture);   //创建纹理对象存储深度缓存
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexStorage2D(GL_TEXTURE_2D, 9, GL_DEPTH_COMPONENT32F, 512, 512);
	//绑定fbo中的缓存到颜色和深度纹理对象
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

	static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers); //绘制第一个cube到fbo的作为窗口帧缓存的纹理
}

void FBO::render(void)
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
	
	cube0Shader.use();  //使用第一个cube的shader绘制
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); //选择绘制的fbo
	glViewport(0, 0, 512, 512);
	glClearBufferfv(GL_COLOR, 0, green);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glUniformMatrix4fv(mvLoc0, 1, 0, glm::value_ptr(mv_matrix));
	glUniformMatrix4fv(projLoc0, 1, 0, glm::value_ptr(proj_matrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);  //绘制立方体

	cube1Shader.use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //解除fbo绑定
	glViewport(0, 0, 1300, 720); //使用刚才绘制的对象作为纹理
	glClearBufferfv(GL_COLOR, 0, blue);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glBindTexture(GL_TEXTURE_2D, color_texture); //选择上面的物体作为纹理数据
	
	glUniformMatrix4fv(projLoc1, 1, 0, glm::value_ptr(proj_matrix));
	glUniformMatrix4fv(mvLoc1, 1, 0, glm::value_ptr(mv_matrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);  //解除纹理绑定

}


