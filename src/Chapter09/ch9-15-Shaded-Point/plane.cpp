#include "Plane.h"
#include "ogl/ktx.h"
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




namespace byhj
{

Plane::Plane()
{

}

Plane::~Plane()
{

}

void Plane::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Plane::Render(GLfloat aspect)
{
	shapeShader.use();

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(100.0f);
	glDrawArrays(GL_POINTS, 0, 4); //绘制四个占200象数的点

	glUseProgram(0);
}


void Plane::Shutdown()
{

}

void Plane::init_buffer()
{


}

void Plane::init_vertexArray()
{

}

void Plane::init_shader()
{
	shapeShader.init();
	shapeShader.attach(GL_VERTEX_SHADER, "shape.vert");
	shapeShader.attach(GL_FRAGMENT_SHADER, "shape.frag");
	shapeShader.link();
	shapeShader.use();
	
}

void Plane::init_texture()
{

}

}