#include <GL/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>

class Shape: public sb6::Application
{
public:
	void init_shader();
	void init();
	void render();
private:
   Shader shapeShader;
};

DECLARE_MAIN(Shape);

void Shape::init_shader(void)
{
	shapeShader.init();
	shapeShader.attach(GL_VERTEX_SHADER, "shape.vert");
	shapeShader.attach(GL_FRAGMENT_SHADER, "shape.frag");
	shapeShader.link();
	shapeShader.use();
}
void Shape::init()
{
	init_shader();
}

void Shape::render(void)
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat one[] = { 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, one);

	glViewport(0, 0, 1300, 900);
	shapeShader.use();
    glPointSize(200.0f);
	glDrawArrays(GL_POINTS, 0, 4); //绘制四个占200象数的点
}

