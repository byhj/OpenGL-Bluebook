#include <GL/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <vector> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
#include "common.h"

GLuint VBO, IBO;   
GLuint VAO;
Shader cubeShader("cube shader");

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

static const GLfloat vertex_positions[] =
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
void init(void)
{

	cubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	cubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	cubeShader.link();
	cubeShader.begin();
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertex_positions),
                     vertex_positions,
                     GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(vertex_indices),
                     vertex_indices,
                     GL_STATIC_DRAW);

   glEnable(GL_CULL_FACE);

}

void display(void)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        static const GLfloat one = 1.0f;
        glClearBufferfv(GL_COLOR, 0, black);

		static float currentTime ;
	currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	for (int i = 0; i < 24; i++) {
	    float time = (float)i + (float)currentTime * 0.3f;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -6.0f)) *
		glm::translate(glm::mat4(1.0), glm::vec3(sinf(2.1f * time)*0.5f, cosf(1.7f*time)*0.5f,
		               sinf(1.3f * time) *cosf(1.5f * time) * 2.0f) );

		mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f)) ;
		mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 21.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		mv_matrix *= glm::translate(glm::mat4(1.0), glm::vec3( sinf(2.1f * time) * 2.0f, cosf(1.7f * time) * 2.0f,
			                        sinf(1.3f * time) * cosf(1.5f * time) * 2.0f) );
		cubeShader.setUniformMatrix4fv("mv_matrix", 1, 0, glm::value_ptr(mv_matrix));
	   glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}
	 glutSwapBuffers(); //强制绘图，去掉会变白色
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);  //视口调整
	static float aspect;
	if (w > h)
	  aspect = float(w) / float(h);
	else
	  aspect = float(h) / float(w);
	glm::mat4 proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);
	cubeShader.setUniformMatrix4fv("proj_matrix", 1, 0, glm::value_ptr(proj_matrix));
}

void idle(void )
{

	glutPostRedisplay();
}

int main(int argc, char ** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //显示模式，重要
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Cube");
	//glewExperimental = GL_TRUE;
    glewInit();

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop(); //循环调用注册函数display
	return 0;
}