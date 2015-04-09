#include <GL/glew.h>
#include <cmath>
#include <gl/freeglut.h>
#include <iostream>
#include <vector> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>

using namespace std;

#include "ktx.h"
Shader  starShader("Star shader");
GLuint time_loc, proj_matrix_loc;

struct  star_t//位置，颜色
{
   float  position[3];
   float  color[3];
};

GLuint          render_prog;
GLuint          star_texture;
GLuint          star_vao;
GLuint          star_buffer;

struct
{
   int timeLoc;
   int proj_matrixLoc;
} uniforms;

enum
{
    NUM_STARS           = 2000
};

// Random number generator
static unsigned int seed = 0x13371337;

static inline float random_float() //随机数
{
    float res;
    unsigned int tmp;

    seed *= 16807;
    tmp = seed ^ (seed >> 4) ^ (seed << 15);
    *((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

    return (res - 1.0f);
}

void init(void)  
{ 
    starShader.init();
	starShader.attach(GL_VERTEX_SHADER, "star.vert");
	starShader.attach(GL_FRAGMENT_SHADER, "star.frag");
	starShader.link();
	starShader.use();
	time_loc = glGetUniformLocation(starShader.program, "time");
	proj_matrix_loc = glGetUniformLocation(starShader.program, "proj_matrix");
    star_texture = ktx::file::load("star.ktx");
	glGenVertexArrays(1, &star_vao);
	glBindVertexArray(star_vao);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glGenBuffers(1, &star_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, star_buffer);
    glBufferData(GL_ARRAY_BUFFER, NUM_STARS * sizeof(star_t), NULL, GL_STATIC_DRAW);
	star_t * star = (star_t *)glMapBufferRange(GL_ARRAY_BUFFER, 0, NUM_STARS * sizeof(star_t),
		                                       GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (int i = 0; i < 1000; i++) {
            star[i].position[0] = (random_float() * 2.0f - 1.0f) * 100.0f;
            star[i].position[1] = (random_float() * 2.0f - 1.0f) * 100.0f;
            star[i].position[2] = random_float();
            star[i].color[0] = 0.8f + random_float() * 0.2f;
            star[i].color[1] = 0.8f + random_float() * 0.2f;
            star[i].color[2] = 0.8f + random_float() * 0.2f;
       }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), (void *)sizeof(float[3]));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

}


void ::(void)
{
      static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
      static const GLfloat one[] = { 1.0f };

      glClearBufferfv(GL_COLOR, 0, black);
      glClearBufferfv(GL_DEPTH, 0, one);

      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE); //开启混合
	  static float currentTime ;
	  currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	  float t = (float)currentTime;
	  t *= 0.1f;
      t -= floor(t);
      glBindVertexArray(star_vao);
	  glUniform1f(time_loc, t);
      glEnable(GL_PROGRAM_POINT_SIZE);
      glDrawArrays(GL_POINTS, 0, NUM_STARS);
	  glutSwapBuffers(); //强制绘图，去掉会变白色

}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);  //视口调整
	static float aspect;
	if (w > h)
	  aspect = float(w) / float(h);
	else
	  aspect = float(h) / float(w);
	glm::mat4 proj_matrix = glm::perspective(60.0f, aspect, 0.1f, 1000.0f);
	glUniformMatrix4fv(proj_matrix_loc, 1, 0, glm::value_ptr(proj_matrix));

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
	glutCreateWindow("Texture");
	//glewExperimental = GL_TRUE;
    glewInit();

	init();
	glutDisplayFunc(::);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop(); //循环调用注册函数display
	return 0;
}