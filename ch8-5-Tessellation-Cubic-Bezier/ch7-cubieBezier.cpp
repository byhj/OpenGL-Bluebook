#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#include <ktx.cpp>
#include <shader.h>

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

struct Window {
	Window(char *t, const int &w, const int &h, const int &px, const int &py)
		:title(t), width(w), height(h), posX(px), posY(py){}
	char *title;
	int width, height;
	int posX, posY;
};
Window window("tunnel", 1300, 900, 300, 0);
Shader cubicBezierShader("texture shader");
Shader cvShader("cv");
glm::vec3 patch_data[16];


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

void load_shader()
{
	cubicBezierShader.attach(GL_VERTEX_SHADER, "cubicBezier.vert");
	cubicBezierShader.attach(GL_TESS_CONTROL_SHADER, "cubicBezier.tcs");
	cubicBezierShader.attach(GL_TESS_EVALUATION_SHADER, "cubicBezier.tes");
	cubicBezierShader.attach(GL_FRAGMENT_SHADER, "cubicBezier.frag");
	cubicBezierShader.link();
	cubicBezierShader.use();
    patch::program  = cubicBezierShader.program;
	patch::mv_matrix_loc = glGetUniformLocation( patch::program, "mv_matrix");
	patch::proj_matrix_loc = glGetUniformLocation(patch::program, "proj_matrix");
	patch::mvp_matrix_loc = glGetUniformLocation(patch::program, "mvp_matrix");

	cvShader.attach(GL_VERTEX_SHADER, "cv.vert");
	cvShader.attach(GL_FRAGMENT_SHADER, "cv.frag");
    cv::program =  cvShader.program;
	cv::draw_color_loc = glGetUniformLocation(cv::program, "draw_color");
	cv::mvp_matrix_loc = glGetUniformLocation(cv::program, "mvp_matrix");

}


void init_vertexArray()
{
	glGenVertexArrays(1, &patch::vao);
	glBindVertexArray(patch::vao);
	glBindBuffer(GL_ARRAY_BUFFER, patch::buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

void init_buffer()
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

void init()
{
	glEnable(GL_DEPTH_TEST);
	load_shader();
	init_buffer();
	init_vertexArray();
}

void render()
{
    static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
    static const GLfloat one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, gray);
    glClearBufferfv(GL_DEPTH, 0, &one);
	glViewport(0, 0, window.width, window.height);
	static float currentTime =  glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
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
    for (int i = 0; i < 16; i++) {
        float fi = (float)i / 16.0f;
        p[i][2] = sinf(t * (0.2f + fi * 0.3f));
    }
	glUnmapBuffer(GL_ARRAY_BUFFER);

    glUseProgram(patch::program);
	glm::mat4 proj_matrix = glm::perspective(50.0f, 1.0f, 0.1f, 1000.0f);
	glm::mat4 mv_matrix  = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -4.0f)) *
		                   glm::rotate(glm::mat4(1.0), t * 10.0f, glm::vec3( 0.0f, 1.0f, 0.0f))*
                           glm::rotate(glm::mat4(1.0), t * 17.0f, glm::vec3(1.0f, 0.0f, 0.0f)) ;
	glUniformMatrix4fv(patch::mv_matrix_loc, 1, GL_FALSE, &mv_matrix[0][0]);
	glUniformMatrix4fv(patch::proj_matrix_loc, 1, GL_FALSE, &proj_matrix[0][0]);
	glUniformMatrix4fv(patch::mvp_matrix_loc, 1, GL_FALSE, &(proj_matrix * mv_matrix)[0][0]);

    if (Key::wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPatchParameteri(GL_PATCH_VERTICES, 16);
    glDrawArrays(GL_PATCHES, 0, 16);

	glUseProgram(cv::program);
    glUniformMatrix4fv(cv::mvp_matrix_loc, 1, GL_FALSE, &(proj_matrix * mv_matrix)[0][0]);
   
	if (Key::show_points)  {
        glPointSize(9.0f);
        glUniform4fv(cv::draw_color_loc, 1, &(glm::vec4(0.2f, 0.7f, 0.9f, 1.0f))[0]);
        glDrawArrays(GL_POINTS, 0, 16);
    }

    if (Key::show_cage) {
        glUniform4fv(cv::draw_color_loc, 1, &(glm::vec4(0.7f, 0.9f, 0.2f, 1.0f))[0]);
        glDrawElements(GL_LINES, 48, GL_UNSIGNED_SHORT, NULL);
    }
	glutSwapBuffers(); //强制绘图，去掉会变白色
}

void idle(void)
{
	glutPostRedisplay();

}

void onKey(unsigned char key, int x, int y)
{
   switch (key) {
             case 'c': Key::show_cage = !Key::show_cage;
                break;
             case 'x': Key::show_points = !Key::show_points;
                break;
             case 'w': Key::wireframe = !Key::wireframe;
                break;
			 case 'p': Key::paused = !Key::paused;
                break;
            default:
                    break;
   };
 
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //显示模式，重要
	glutInitWindowPosition(window.posX, window.posY);
	glutInitWindowSize(window.width, window.height);
	glutCreateWindow(window.title);
	GLenum status = glewInit();
	if (status != GLEW_OK)
		cerr << glewGetErrorString(status) << endl;
	init();
	glutDisplayFunc(render);
	glutIdleFunc(idle);
	glutKeyboardFunc(onKey);
	glutMainLoop(); //循环调用注册函数display
	return 0;
}