
#include <GL/glew.h>
#include <GL/wglew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <vector> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

#include "common.h"

enum
{
    WORKGROUP_SIZE  = 256,
    NUM_WORKGROUPS  = 64,
    FLOCK_SIZE      = (NUM_WORKGROUPS * WORKGROUP_SIZE)
};
    GLuint      flock_update_program;
    GLuint      flock_render_program;

    GLuint      flock_buffer[2];

    GLuint      flock_render_vao[2];
    GLuint      geometry_buffer;

    struct flock_member
    {
        glm::vec3 position;
        unsigned int : 32;
        glm::vec3 velocity;
        unsigned int : 32;
    };

    struct
    {
        struct
        {
            GLint       goal;
        } update;
        struct
        {
            GLuint      mvp;
        } render;
    } uniforms;

    GLuint      frame_index;
void init(void)
{
        // This is position and normal data for a paper airplane
        static const glm::vec3 geometry[] =
        {
            // Positions
            glm::vec3(-5.0f, 1.0f, 0.0f),
            glm::vec3(-1.0f, 1.5f, 0.0f),
            glm::vec3(-1.0f, 1.5f, 7.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 10.0f),
            glm::vec3(1.0f, 1.5f, 0.0f),
            glm::vec3(1.0f, 1.5f, 7.0f),
            glm::vec3(5.0f, 1.0f, 0.0f),

            // Normals
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.107f, -0.859f, 0.00f),
            glm::vec3(0.832f, 0.554f, 0.00f),
            glm::vec3(-0.59f, -0.395f, 0.00f),
            glm::vec3(-0.832f, 0.554f, 0.00f),
            glm::vec3(0.295f, -0.196f, 0.00f),
            glm::vec3(0.124f, 0.992f, 0.00f),
        };

        load_shaders();

        glGenBuffers(2, flock_buffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, flock_buffer[0]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, FLOCK_SIZE * sizeof(flock_member), NULL, GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, flock_buffer[1]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, FLOCK_SIZE * sizeof(flock_member), NULL, GL_DYNAMIC_COPY);

        int i;

        glGenBuffers(1, &geometry_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, geometry_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);

        glGenVertexArrays(2, flock_render_vao);

        for (i = 0; i < 2; i++)
        {
            glBindVertexArray(flock_render_vao[i]);
            glBindBuffer(GL_ARRAY_BUFFER, geometry_buffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)(8 * sizeof(glm::vec3)));

            glBindBuffer(GL_ARRAY_BUFFER, flock_buffer[i]);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(flock_member), NULL);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(flock_member), (void *)sizeof(glm::vec4));
            glVertexAttribDivisor(2, 1);
            glVertexAttribDivisor(3, 1);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
        }

        glBindBuffer(GL_ARRAY_BUFFER, flock_buffer[0]);
        flock_member * ptr = reinterpret_cast<flock_member *>(glMapBufferRange(GL_ARRAY_BUFFER, 0, FLOCK_SIZE * sizeof(flock_member), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

        for (i = 0; i < FLOCK_SIZE; i++)
        {
            ptr[i].position = (glm::vec3::random() - glm::vec3(0.5f)) * 300.0f;
            ptr[i].velocity = (glm::vec3::random() - glm::vec3(0.5f));
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
 }

float aspect;
void display(void)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glUseProgram(program);
	glm::vec3 goal = glm::vec3(sinf(t * 0.34f),
                                       cosf(t * 0.29f),
                                       sinf(t * 0.12f) * cosf(t * 0.5f));

     goal = goal * glm::vec3(35.0f, 25.0f, 60.0f);

    glUniform3fv(uniforms.update.goal, 1, goal);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, flock_buffer[frame_index]);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, flock_buffer[frame_index ^ 1]);

    glDispatchCompute(NUM_WORKGROUPS, 1, 1);
	
	static float f ;
	f = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	glm::mat4 proj_matrix = glm::perspective(60.0f, aspect, 0.1f, 1000.0f);

    glm::mat4 mv_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -400.0f),
                                              glm::vec3(0.0f, 0.0f, 0.0f),
                                              glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 mvp = proj_matrix * mv_matrix;
	glUniformMatrix4fv(uniforms.render.mvp, 1, GL_FALSE, glm::value_ptr(mvp));
    glBindVertexArray(flock_render_vao[frame_index]);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 8, FLOCK_SIZE);

        frame_index ^= 1;
	 glutSwapBuffers(); //强制绘图，去掉会变白色
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);  //视口调整
	if (w > h)
	  aspect = float(w) / float(h);
	else
	  aspect = float(h) / float(w);

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
	glutCreateWindow("Tessallation Shaders");
	//glewExperimental = GL_TRUE;
    glewInit();
	init();
    glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop(); //循环调用注册函数display
	return 0;
}