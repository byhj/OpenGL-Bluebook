#include <GL/glew.h>

#include "ogl/oglApp.h"
#include "ogl/shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>



class Smooth : public byhj::Application
{
public:
	void v_Init();
	void v_Render();

	void init_shader();
	void init_buffer();
	void init_vertexArray(); 
private:
	GLuint VBO, IBO, UBO;   
	GLuint VAO, program;
	Shader cubeShader;
	GLuint mv_location, proj_location;;
};
CALL_MAIN(Smooth);

static const GLushort ElementData[] =
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

static const GLfloat VertexData[] =
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


void Smooth::init_shader()
{
	cubeShader.init();
	cubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	cubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	cubeShader.link();
	cubeShader.use();
	program = cubeShader.GetProgram();
	mv_location= glGetUniformLocation(program, "mv_matrix");
	proj_location = glGetUniformLocation(program, "proj_matrix");
}

void Smooth::init_buffer()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Smooth::init_vertexArray() 
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glEnableVertexAttribArray(0);
}

void Smooth::v_Init()
{  
   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   init_shader();
   init_buffer();
   init_vertexArray();
}


void Smooth::v_Render()
{
	static const GLfloat orange[] = { 0.6f, 0.4f, 0.1f, 1.0f };
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_COLOR, 0, orange);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glm::mat4 proj_matrix = glm::perspective(50.0f, GetAspect(), 0.1f, 1000.0f);
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, &proj_matrix[0][0]);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

	float currentTime = glfwGetTime() / 100.0f;
	float f = (float)currentTime * 0.3f;
    currentTime = 3.15;
    glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f))
                        * glm::rotate(glm::mat4(1.0f), (float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f)) 
						* glm::rotate(glm::mat4(1.0f), (float)currentTime * 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(mv_location, 1, GL_FALSE, &mv_matrix[0][0]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

