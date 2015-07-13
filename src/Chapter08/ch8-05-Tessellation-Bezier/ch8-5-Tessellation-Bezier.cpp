#include <ogl/ktx.cpp>
#include <ogl/shader.h>
#include "ogl/oglApp.h"
#include <ogl/vmath.h>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



namespace Key {
	bool   show_points = false;
    bool   show_cage = false;
    bool   wireframe = false;
    bool   paused = false;
};

namespace patch {
	GLuint mv_matrix_loc;
	GLuint proj_matrix_loc;
	GLuint mvp_matrix_loc;
	GLuint vao;
	GLuint buffer;
	GLuint program;
};

namespace cv {
	GLuint draw_color_loc;
	GLuint mvp_matrix_loc;
	GLuint cage_indices;
	GLuint program;
};


class IndexCube: public byhj::Application
{
public:
	IndexCube() {};
	~IndexCube(){}

	void v_Init();
	void v_Render();
	void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
	{	
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		if (key == GLFW_KEY_C && action == GLFW_PRESS)
			 Key::show_cage = !Key::show_cage;

		if (key == GLFW_KEY_X && action == GLFW_PRESS)
			Key::show_points = !Key::show_points;

		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		    Key::wireframe = !Key::wireframe;

		if (key == GLFW_KEY_P && action == GLFW_PRESS)
			Key::paused = !Key::paused;
	}
	void init_shader();
	void init_buffer();
	void init_vertexArray();

public:
	Shader cubicBezierShader;
	Shader cvShader;
	glm::vec3 patch_data[16];
};

CALL_MAIN(IndexCube);

static const float patch_initializer[] =
    {
        -1.0f,  -1.0f,  0.0f,
        -0.33f, -1.0f,  0.0f,
         0.33f, -1.0f,  0.0f,
         1.0f,  -1.0f,  0.0f,

        -1.0f,  -0.33f, 0.0f,
        -0.33f, -0.33f, 0.0f,
         0.33f, -0.33f, 0.0f,
         1.0f,  -0.33f, 0.0f,

        -1.0f,   0.33f, 0.0f,
        -0.33f,  0.33f, 0.0f,
         0.33f,  0.33f, 0.0f,
         1.0f,   0.33f, 0.0f,

        -1.0f,   1.0f,  0.0f,
        -0.33f,  1.0f,  0.0f,
         0.33f,  1.0f,  0.0f,
         1.0f,   1.0f,  0.0f,
};

static const GLushort indices[] =
    {
        0, 1, 1, 2, 2, 3,
        4, 5, 5, 6, 6, 7,
        8, 9, 9, 10, 10, 11,
        12, 13, 13, 14, 14, 15,

        0, 4, 4, 8, 8, 12,
        1, 5, 5, 9, 9, 13,
        2, 6, 6, 10, 10, 14,
        3, 7, 7, 11, 11, 15
};

void IndexCube::init_shader()
{
	cubicBezierShader.attach(GL_VERTEX_SHADER, "cubicBezier.vert");
	cubicBezierShader.attach(GL_TESS_CONTROL_SHADER, "cubicBezier.tcs");
	cubicBezierShader.attach(GL_TESS_EVALUATION_SHADER, "cubicBezier.tes");
	cubicBezierShader.attach(GL_FRAGMENT_SHADER, "cubicBezier.frag");
	cubicBezierShader.link();
	cubicBezierShader.use();
    patch::program  = cubicBezierShader.GetProgram();
	patch::mv_matrix_loc = glGetUniformLocation( patch::program, "mv_matrix");
	patch::proj_matrix_loc = glGetUniformLocation(patch::program, "proj_matrix");
	patch::mvp_matrix_loc = glGetUniformLocation(patch::program, "mvp_matrix");

	cvShader.attach(GL_VERTEX_SHADER, "cv.vert");
	cvShader.attach(GL_FRAGMENT_SHADER, "cv.frag");
    cv::program =  cvShader.GetProgram();
	cv::draw_color_loc = glGetUniformLocation(cv::program, "draw_color");
	cv::mvp_matrix_loc = glGetUniformLocation(cv::program, "mvp_matrix");

}


void IndexCube::init_vertexArray()
{
	glGenVertexArrays(1, &patch::vao);
	glBindVertexArray(patch::vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cv::cage_indices);
	glBindBuffer(GL_ARRAY_BUFFER, patch::buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

void IndexCube::init_buffer()
{
	glGenBuffers(1, &patch::buffer);
    glBindBuffer(GL_ARRAY_BUFFER, patch::buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(patch_data), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &cv::cage_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cv::cage_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IndexCube::v_Init()
{
	glEnable(GL_DEPTH_TEST);
	init_shader();
	init_buffer();
	init_vertexArray();
}

void IndexCube::v_Render()
{
    static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
    static const GLfloat one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, gray);
    glClearBufferfv(GL_DEPTH, 0, &one);

	glViewport(0, 0, GetScreenWidth(), GetScreenHeight());
	static float currentTime =  glfwGetTime() / 1000.0f;
    static double last_time = 0.0;
    static double total_time = 0.0;

	currentTime += 0.05;
        if (!Key::paused)
            total_time += (currentTime - last_time);
        last_time = currentTime;
    float t = (float)total_time;
	    
    glm::vec3 * p = (glm::vec3 *)glMapBufferRange(GL_ARRAY_BUFFER,
		                         0, sizeof(patch_data), 
		                         GL_MAP_WRITE_BIT | 
							     GL_MAP_INVALIDATE_BUFFER_BIT);

    memcpy(p, patch_initializer, sizeof(patch_initializer));
    for (int i = 0; i < 16; i++) 
	{
        float fi = (float)i / 16.0f;
        p[i][2] = sinf(t * (0.2f + fi * 0.3f));
    }
	glUnmapBuffer(GL_ARRAY_BUFFER);

    glUseProgram(patch::program);

	vmath::mat4 proj_matrix = vmath::perspective(50.0f, GetAspect(),1.0f, 1000.0f);
	vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -4.0f) 
		                  * vmath::rotate(t * 10.0f, 0.0f, 1.0f, 0.0f)
		                  * vmath::rotate(t * 17.0f, 1.0f, 0.0f, 0.0f);

	glUniformMatrix4fv(patch::mv_matrix_loc, 1, GL_FALSE, mv_matrix);
	glUniformMatrix4fv(patch::proj_matrix_loc, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(patch::mvp_matrix_loc, 1, GL_FALSE, (proj_matrix * mv_matrix));

    if (Key::wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPatchParameteri(GL_PATCH_VERTICES, 16);
    glDrawArrays(GL_PATCHES, 0, 16);

	glUseProgram(cv::program);
    glUniformMatrix4fv(cv::mvp_matrix_loc, 1, GL_FALSE, &(proj_matrix * mv_matrix)[0][0]);
   
	if (Key::show_points)  
	{
        glPointSize(9.0f);
        glUniform4fv(cv::draw_color_loc, 1, &(glm::vec4(0.2f, 0.7f, 0.9f, 1.0f))[0]);
        glDrawArrays(GL_POINTS, 0, 16);
    }

    if (Key::show_cage) 
	{
        glUniform4fv(cv::draw_color_loc, 1, &(glm::vec4(0.7f, 0.9f, 0.2f, 1.0f))[0]);
        glDrawElements(GL_LINES, 48, GL_UNSIGNED_SHORT, NULL);
    }
}

