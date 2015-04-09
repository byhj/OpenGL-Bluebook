#include <GL/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>

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
	-0.75f, -0.75f, -0.75f,
	-0.75f,  0.75f, -0.75f,
	0.75f, -0.75f, -0.75f,
	0.75f,  0.75f, -0.75f,
	0.75f, -0.75f,  0.75f,
	0.75f,  0.75f,  0.75f,
	-0.75f, -0.75f,  0.75f,
	-0.75f,  0.75f,  0.75f,
};


class Scrissor: public sb6::Application
{
	void init_shader();
	void init();
	void render();
	void init_buffer();
	void init_vertexArray();
private:
   GLuint VBO, IBO;   
   GLuint VAO, UBO;
   Shader cubeShader;
};

DECLARE_MAIN(Scrissor);

void Scrissor::init_shader()
{
	cubeShader.attach(GL_VERTEX_SHADER, "scissor.vert");
	cubeShader.attach(GL_GEOMETRY_SHADER, "scissor.geom");
	cubeShader.attach(GL_FRAGMENT_SHADER, "scissor.frag");
	cubeShader.link();
	cubeShader.use();
}

void Scrissor::init_buffer()
{    
	glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW); //多次修改
}

void Scrissor::init_vertexArray()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void Scrissor::init(void)
{
   init_shader();
   init_buffer();
   init_vertexArray();
   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
}
const float width = 1300.0f;
const float height = 900.0f;

void Scrissor::render(void)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;
	glDisable(GL_SCISSOR_TEST);

    glClearBufferfv(GL_COLOR, 0, black);  //清除frameBuffer缓存
	glClearBufferfv(GL_DEPTH, 0, &one);

	cubeShader.use();
	glBindVertexArray(VAO);
	glEnable(GL_SCISSOR_TEST);
	int scissor_width = (7 * width) / 16; // 7/16window
	int scissor_height = (7 * height) / 16;
	// Lower left...
	glScissorIndexed(0,     //index
		             0, 0, //left bottom
					 scissor_width, scissor_height); // width height
	// Lower right...
	glScissorIndexed(1, 
		            width - scissor_width, 0,
		            width - scissor_width, scissor_height);
	//Upper left...
	glScissorIndexed(2, 
		             0, height - scissor_height,
		             scissor_width, scissor_height);
	        // Upper right...
    glScissorIndexed(3,
                     width - scissor_width, height- scissor_height,
                     scissor_width, scissor_height);

    glm::mat4 proj_matrix = glm::perspective(45.0f, width / height, 0.1f, 1000.0f);
    static float currentTime ;
	currentTime = glfwGetTime() / 100.0f;
	float f = (float)currentTime * 0.3f;

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO); //binding  = 0
	  glm::mat4 * mv_matrix_array = (glm::mat4 *) glMapBufferRange( 
		                   GL_UNIFORM_BUFFER,
		                   0, 4 * sizeof(glm::mat4), 
		                   GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT
		                  );
		 for (int i = 0; i < 4; i++)  
		 {
            mv_matrix_array[i] = proj_matrix 
				               * glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -2.0f)) 
                               * glm::rotate(glm::mat4(1.0), (float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f)) 
		                       * glm::rotate(glm::mat4(1.0), (float)currentTime * 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));
          }
     glUnmapBuffer(GL_UNIFORM_BUFFER);

     glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

}
