#include <GL/glew.h>

#include "ogl/oglApp.h"
#include "ogl/shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shape: public byhj::Application
{
public:
	void init_shader();
	void v_Init();
	void v_Render();

private:
   Shader shapeShader;
};

CALL_MAIN(Shape);

void Shape::init_shader(void)
{
	shapeShader.init();
	shapeShader.attach(GL_VERTEX_SHADER, "shape.vert");
	shapeShader.attach(GL_FRAGMENT_SHADER, "shape.frag");
	shapeShader.link();
	shapeShader.use();
}

void Shape::v_Init()
{
	init_shader();
}

void Shape::v_Render(void)
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat one[] = { 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, one);

	glViewport(0, 0, 1300, 900);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(200.0f);
	glDrawArrays(GL_POINTS, 0, 4); //绘制四个占200象数的点
}

